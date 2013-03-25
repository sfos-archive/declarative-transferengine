#ifndef DECLARATIVESHARE_H
#define DECLARATIVESHARE_H


#include <QDeclarativeItem>
#include "transfertypes.h"

/**
 * QML Share element is a non-visual item which can be used for starting a share operation from
 * the QML code. Basic usage:
 *
 * import Sailfish.TransferEngine 1.0
 *
 * SailfishShare {
 *      source: url
 *      mimeType: "image/jpeg"
 *      serviceId: "Facebook" // This must be retrieved from ShareMethodList
 *      metadataStripped: true // Get rid of metadata
 *      userData: {"accountId": 6, "description": "Lorem ipsum, etc" } // User data
 *  }
 *
 *  ...
 *  // Start sharing
 *  onClicked: shareItem.start()
 *
 * Alternatively, if the required plugin supports non-file sharing, you may specify the content to
 * be shared instead of the file url. This is done by specifying a "content" object map with a
 * "data" value, and optionally, a "name" value:
 *
 * SailfishShare {
 *      content: {"data": "BEGIN:VCARD\nVERSION:4.0\nN:Smith;John;;;\nEND:VCARD", "name": "JohnSmith.vcf"}
 * }
 *
 *
 *  The following userData keys are handled by the Transfer Engine:
 *  "accountId" - The id of the account to be used for sharing
 *  "description" - The description, which is stored by transfer engine. Depending
 *                  on the service, this might be used.
 *  "title" - The title for the media, which is stored by transfer engine. Depending
 *            on the service, this might be used.
 *
 *  All the other keys are not stored by transfer engine, but the user data is passed
 *  to the C++ share plugin and it's in plugin reponsible to use user data.
 */
class DeclarativeSharePrivate;
class DeclarativeShare : public QDeclarativeItem
{
    Q_OBJECT
    Q_ENUMS(Status)

    Q_PROPERTY(QUrl source WRITE setSource READ source NOTIFY sourceChanged)
    Q_PROPERTY(QVariantMap content WRITE setContent READ content NOTIFY contentChanged)
    Q_PROPERTY(QString serviceId WRITE setServiceId READ serviceId NOTIFY serviceIdChanged)
    Q_PROPERTY(QString mimeType WRITE setMimeType READ mimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(QVariantMap userData WRITE setUserData READ userData NOTIFY userDataChanged)
    Q_PROPERTY(bool metadataStripped WRITE setMetadataStripped READ metadataStripped NOTIFY metadataStrippedChanged)
    Q_PROPERTY(bool notificationsEnabled WRITE setNotificationsEnabled READ notificationsEnabled NOTIFY notificationsEnabledChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)


public:

    enum Status {
        NotStarted          = TransferEngineData::NotStarted,
        TransferStarted     = TransferEngineData::TransferStarted,
        TransferCanceled    = TransferEngineData::TransferCanceled,
        TransferFinished    = TransferEngineData::TransferFinished,
        TransferInterrupted = TransferEngineData::TransferInterrupted
    };

    DeclarativeShare(QDeclarativeItem *parent = 0);
    ~DeclarativeShare();

    void setContent(const QVariantMap &content);
    QVariantMap content() const;

    void setSource(const QUrl source);
    QUrl source() const;

    void setServiceId(const QString &id);
    QString serviceId() const;

    void setMimeType(const QString &mimeType);
    QString mimeType() const;

    void setUserData(const QVariantMap &userData);
    QVariantMap userData() const;

    void setMetadataStripped(bool strip);
    bool metadataStripped() const;

    void setNotificationsEnabled(bool enable);
    bool notificationsEnabled() const;

    qreal progress() const;

    DeclarativeShare::Status status() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void cancel();

Q_SIGNALS:
    void sourceChanged();
    void contentChanged();
    void serviceIdChanged();
    void mimeTypeChanged();
    void userDataChanged();
    void metadataStrippedChanged();
    void notificationsEnabledChanged();
    void progressChanged(qreal progress);
    void statusChanged(Status status);

private:
    DeclarativeSharePrivate *d_ptr;
    Q_DECLARE_PRIVATE(DeclarativeShare)
    Q_DISABLE_COPY(DeclarativeShare)
};

QML_DECLARE_TYPE(DeclarativeShare)

#endif

