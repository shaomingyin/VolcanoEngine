//
//
#ifndef VOLCANO_GAME_RESOURCE_HPP
#define VOLCANO_GAME_RESOURCE_HPP

#include <QUrl>
#include <QIODevice>
#include <QNetworkReply>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class Resource: public Component {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)

public:
    enum Status {
        None = 0,
        Loading,
        Ready,
        Error
    };
    Q_ENUM(Status)

    class Handler {
    public:
        Handler(void) = default;
        virtual ~Handler(void) = default;

    public:
        QNetworkReply *createRequest(QIODevice *io);

    protected:
        virtual QNetworkReply *createRequestInternal(QIODevice *io) = 0;
    };

public:
    Resource(QObject *parent = nullptr);
    ~Resource(void) override;

public:
    const QUrl &source(void) const;
    void setSource(const QUrl &v);
    Status status(void) const;
    qreal progress(void) const;

signals:
    void sourceChanged(const QUrl &v);
    void statusChanged(Status v);
    void progressChanged(qreal v);

protected:
    void setStatus(Status v);
    void setProgress(qreal v);
    virtual void onSourceChanged(const QUrl &v) = 0;

private:
    QUrl m_source;
    Status m_status;
    qreal m_progress;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_RESOURCE_HPP
