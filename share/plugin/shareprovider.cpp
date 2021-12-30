/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#include <nemo-dbus/dbus.h>
#include <QCoreApplication>
#include <QDBusAbstractAdaptor>
#include <QDBusConnection>
#include <QMetaType>
#include <QQmlInfo>

#include "mimetypefilter.h"
#include "shareprovider.h"

namespace {

const auto NameKey = QLatin1String("name");
const auto DataKey = QLatin1String("data");

} // namespace

class ShareProvider::Adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.sailfishos.share")

public:
    Adaptor(ShareProvider *shareProvider)
        : QDBusAbstractAdaptor(shareProvider)
        , m_shareProvider(shareProvider)
    {
    }

public slots:
    void share(QVariantMap configuration, const QDBusMessage &message)
    {
        // Most of the errors below can not normally happen but because this is
        // a D-Bus API we must sanitize the data in case it comes from a
        // different source than expected.
        QVariantList resources;
        QString mimeType;

        for (auto it = configuration.constBegin(); it != configuration.constEnd(); ++it) {
            const QVariant &value = NemoDBus::demarshallDBusArgument(it.value());

            if (it.key() == QLatin1String("resources")) {
                resources = value.toList();
            } else if (it.key() == QLatin1String("mimeType")) {
                mimeType = value.toString();
            } else if (it.key() == QLatin1String("title")) {
                // ignore
            } else if (it.key() == QLatin1String("selectedTransferMethodInfo")) {
                // ignore
            } else {
                qmlInfo(m_shareProvider) << "Unrecognized key '" << it.key() << "' with value:" << value;
            }
        }

        bool valid = false;
        if (MimeTypeFilter::validMimeTypeFilter(mimeType)) {
            if (MimeTypeFilter::allAllowed(m_shareProvider->m_capabilities)) {
                valid = true;
            } else {
                for (const QString &filter : m_shareProvider->m_capabilities) {
                    if (MimeTypeFilter::filterAcceptsMimeType(filter, mimeType)) {
                        valid = true;
                        break;
                    }
                }
            }
        }

        if (!valid) {
            qmlInfo(m_shareProvider) << "Received invalid mime type: " << mimeType;
            QDBusConnection::sessionBus().send(
                    message.createErrorReply(QDBusError::InvalidArgs, "Invalid mimeType value"));
            return;
        }

        // Go through resources and check the content before triggering sharing:
        // Although they should come from the plugin, it's possible that something else
        // calls the interface and we need to be prepared so let's use a filtered list.
        QVariantList filteredResources;
        for (const QVariant &resource : resources) {
            if (resource.canConvert<QVariantMap>()) {
                QString name;
                QString data;
                QAssociativeIterable iterable = resource.value<QAssociativeIterable>();
                for (auto it = iterable.begin(); it != iterable.end(); ++it) {
                    if (it.key() == NameKey) {
                        name = it.value().toString();
                    } else if (it.key() == DataKey) {
                        data = it.value().toString();
                    } else if (it.key() == QLatin1String("linkTitle")) {
                        // ignore
                    } else if (it.key() == QLatin1String("status")) {
                        // ignore
                    } else if (it.key() == QLatin1String("type")) {
                        // ignore
                    } else {
                        qmlInfo(m_shareProvider) << "Got unknown key for resource map '" << it.key() << "'";
                    }
                }
                if (!name.isEmpty() && !data.isEmpty())
                    filteredResources << QVariant::fromValue(ShareResource::stringResource(name, data));
                else
                    qmlInfo(m_shareProvider) << "Shared resource map did not contain valid 'name' or 'data' values";
            } else if (static_cast<QMetaType::Type>(resource.type()) == QMetaType::QString) {
                QString path = resource.toString();
                if (!path.startsWith('/'))
                    qmlInfo(m_shareProvider) << "Received non-absolute path: " << path;
                else
                    filteredResources << QVariant::fromValue(ShareResource::filePathResource(path));
            } else {
                qmlInfo(m_shareProvider) << "Unknown resource type '"
                                      << static_cast<QMetaType::Type>(resource.type()) << "' received";
            }
        }

        if (filteredResources.isEmpty()) {
            qmlInfo(m_shareProvider) << "No usable resources";
            QDBusConnection::sessionBus().send(
                    message.createErrorReply(QDBusError::InvalidArgs, "Invalid resources value"));
            return;
        }

        emit m_shareProvider->triggered(filteredResources);
    }

private:
    ShareProvider *m_shareProvider;
};

/*!
    \qmltype ShareProvider
    \inqmlmodule Sailfish.Share

    Accepts shared files and signals \l ShareProvider::triggered when another
    application shares with this application.

    Use this item to implement sharing in your application. You need to define
    the sharing method in desktop entry file. See the example below.

    \code
        [Desktop Entry]
        Type=Application
        Name=My application
        Icon=my-application
        Exec=my-application
        X-Share-Methods=images;

        [X-Sailjail]
        ApplicationName=MyApplication
        OrganizationName=org.example
        Permissions=Internet;UserDirs;RemovableMedia

        [X-Share Method images]
        Description=Send to friends
        Capabilities=image/png;image/jpeg
        SupportsMultipleFiles=yes
    \endcode

    The changes you need to make to the application are addition of share
    methods list and definitions of those share methods.

    Depending on the locations of files you expect to receive, you should
    define appropriate permissions for your application in \c X-Sailjail
    section. Files common to all applications are stored on device in
    directories allowed by \c UserDirs permission or one of the more specific
    directory permissions. Similarly files stored on memory card can be
    accessed with \c RemovableMedia permission. It is possible that an
    application tries to share a file private to that application and that may
    fail as your application may not have access to it.

    Each share method must be listed in \c X-Share-Methods key and have its own
    section. You may supply a short description of the sharing method to be
    displayed on sharing dialog. It can be localised with desktop entry style
    localisation keys, e.g. \c Description[fi] would define Finnish version of
    \c Description which is loaded when device language is set to Finnish. If a
    suitable translated version can not be found, it defaults to non-localised
    version.

    You need to handle registering D-Bus name on session bus in your
    application. Do not use \c DBusInterface for that purpose to avoid race
    conditions with registering objects on D-Bus. Use C++ to do it race-free
    instead.

    Example:

    \code
    int main(int argc, char *argv[])
    {
        QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
        QScopedPointer<QQuickView> view(SailfishApp::createView());

        // ...

        view->setSource(SailfishApp::pathToMainQml());

        // Registering the service after QML is loaded
        QDBusConnection::sessionBus().registerService("org.example.MyApplication");

        return app->exec();
    }
    \endcode

    Place ShareProvider directly under your \c ApplicationWindow and define the
    same method name value there. In case of the example above, this would be
    appropriate:

    \qml
    ApplicationWindow {
        id: root

        // ...

        ShareProvider {
            method: "images"
            capabilities: ["image/png", "image/jpeg"]

            onTriggered: {
                root.activate() // Show window
                // Pass resources to handling code. A dialog allows to
                // query more information from user before acting on the data.
                var dialog = pageStack.push("SendPicturesDialog.qml", { 'resources': resources })
            }
        }
    }
    \endqml

    You may register multiple share methods by defining multiple
    \c{X-Share Method} sections and ShareProvider items. Use a different method
    name for each section and use the same name in corresponding item on QML
    code.

    To make your application robust, don't trust that the resources contain
    what you expect. Validating them before use is a good idea.
*/

ShareProvider::ShareProvider(QObject *parent)
    : QObject(parent)
{
    new Adaptor(this);
}

ShareProvider::~ShareProvider()
{
}

QString ShareProvider::dbusPath() const
{
    return m_method.isEmpty() ? QString() : QStringLiteral("/share/%1").arg(m_method);
}

void ShareProvider::registerObject()
{
    if (!dbusPath().isEmpty()) {
        auto bus = QDBusConnection::sessionBus();
        auto current = bus.objectRegisteredAt(dbusPath());
        if (!current) {
            if (!bus.registerObject(dbusPath(), this)) {
                qmlInfo(this) << "Failed to register sharing on session D-Bus path '" << dbusPath() << "'";
            } else {
                current = this;
            }
        } else if (current != this) {
            qmlInfo(this) << "Another object already registered on session D-Bus path '" << dbusPath() << "'";
            qmlInfo(this) << "ShareProvider items must have different method names";
        }
        if (current != this)
            qmlInfo(this) << "Sharing with '" << m_method << "' method will not work!";
    }
}

void ShareProvider::registerService() const
{
    if (m_registerName) {
        auto name = QStringLiteral("%1.%2")
                .arg(QCoreApplication::instance()->organizationName())
                .arg(QCoreApplication::instance()->applicationName());
        if (!QDBusConnection::sessionBus().registerService(name)) {
            qmlInfo(this) << "Failed to register '" << name << "' bus name on session D-Bus";
            qmlInfo(this) << "Sharing to this app will not work!";
        }
    }
}

void ShareProvider::classBegin()
{
}

void ShareProvider::componentComplete()
{
    registerObject();
    registerService();
    m_complete = true;
}

/*!
    \qmlproperty string ShareProvider::method

    Sharing method name as defined in desktop entry file.

    Set this to the same value as listed in \c X-Share-Methods list and in
    \c{X-Share Method} section name in the desktop entry file of the
    application.
*/
QString ShareProvider::method() const
{
    return m_method;
}

void ShareProvider::setMethod(const QString &method)
{
    if (m_method != method) {
        auto bus = QDBusConnection::sessionBus();
        if (!dbusPath().isEmpty() && bus.objectRegisteredAt(dbusPath()) == this)
            bus.unregisterObject(dbusPath());
        m_method = method;
        if (m_complete)
            registerObject();
        emit methodChanged();
    }
}

/*!
    \qmlproperty bool ShareProvider::registerName

    Set to true to automatically register D-Bus bus name on session bus.

    This works only for simple use cases, i.e. when you have only one
    ShareProvider in the application and it doesn't provide D-Bus service for
    anything else. Otherwise register your bus name in C++ after the
    ShareProvider items have been completed.
*/
bool ShareProvider::registerName() const
{
    return m_registerName;
}

void ShareProvider::setRegisterName(bool registerName)
{
    if (m_registerName != registerName) {
        m_registerName = registerName;
        if (m_complete)
            registerService();
        emit registerNameChanged();
    }
}

/*!
    \qmlproperty list ShareProvider::capabilities

    List of mime types that this sharing method accepts.

    If you want to enforce checking of reported mime types while sharing,
    set this to the same value as \c{X-Share Method <method>/Capabilities}.
    E.g. if \c{Capabilities} is \c "image/png;image/jpeg" set this to
    \c{["image/png", "image/jpeg"]}.

    If this is left empty, set to \c undefined, or one of the types is \c "*",
    no checking of mime types takes place.

    Note that you should not trust that the resource content matches to the
    reported mime type. This only checks that the reported mime type matches
    the capabilities set here.

    See also \l ShareAction::mimeType.
*/
QStringList ShareProvider::capabilities() const
{
    return m_capabilities;
}

void ShareProvider::setCapabilities(const QStringList &capabilities)
{
    QStringList filteredCapabilities;
    for (const QString &mimeType : capabilities) {
        if (!MimeTypeFilter::validMimeTypeFilter(mimeType))
            qmlInfo(this) << "Invalid mime type for capabilities: " << mimeType;
        else
            filteredCapabilities.append(mimeType);
    }

    if (m_capabilities != filteredCapabilities) {
        m_capabilities = filteredCapabilities;
        emit capabilitiesChanged();
    }
}

void ShareProvider::resetCapabilities()
{
    setCapabilities(QStringList());
}

/*!
    \qmlsignal ShareProvider::triggered(list resources)

    Signaled when this share method is used.

    The signal handler is \c onTriggered. The argument is a list of resources
    that were shared to the application. See \l ShareResource for more
    information about resources.
*/
#include "shareprovider.moc"
