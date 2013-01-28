#ifndef DECLARATIVESHARE_H
#define DECLARATIVESHARE_H


#include <QDeclarativeItem>
#include "transfertypes.h"


class DeclarativeSharePrivate;
class DeclarativeShare : public QDeclarativeItem
{
    Q_OBJECT
    Q_ENUMS(Status)

    Q_PROPERTY(QUrl source WRITE setSource READ source NOTIFY sourceChanged)
    Q_PROPERTY(QString title WRITE setTitle READ title NOTIFY titleChanged)
    Q_PROPERTY(QString description WRITE setDescription READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString serviceId WRITE setServiceId READ serviceId NOTIFY serviceIdChanged)
    Q_PROPERTY(quint32 accountId WRITE setAccountId READ accountId NOTIFY accountIdChanged)
    Q_PROPERTY(QString mimeType WRITE setMimeType READ mimeType NOTIFY mimeTypeChanged)
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

    void setSource(const QUrl source);
    QUrl source() const;

    void setTitle(const QString &title);
    QString title() const;

    void setDescription(const QString &description);
    QString description() const;

    void setServiceId(const QString &id);
    QString serviceId() const;

    void setAccountId(quint32 id);
    quint32 accountId() const;

    void setMimeType(const QString &mimeType);
    QString mimeType() const;

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
    void titleChanged();
    void descriptionChanged();
    void serviceIdChanged();
    void accountIdChanged();
    void mimeTypeChanged();
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

