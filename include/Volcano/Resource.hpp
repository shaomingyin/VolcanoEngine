//
//
#ifndef VOLCANO_RESOURCE_HPP
#define VOLCANO_RESOURCE_HPP

#include <QUrl>
#include <QCache>
#include <QQmlEngine>
#include <QQmlContext>

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>

VOLCANO_BEGIN

class Resource: public Node
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)

public:
    enum State {
        StateFree = 0,
        StateDirty,
        StateInitializing,
        StateLoading,
        StateReady,
        StateError
    };
    Q_ENUM(State)

public:
    Resource(Node *parent = nullptr);
    ~Resource(void) override;

public:
    const QUrl &source(void) const;
    virtual void setSource(const QUrl &v);
    State state(void) const;
    qreal progress(void) const;

signals:
    void sourceChanged(const QUrl &url);
    void stateChanged(void);
    void progressChanged(void);

protected:
    void onTick(float elapsed) override;
    virtual void onSourceChanged(const QUrl &url) = 0;
    QUrl baseUrl(void) const;
    void setState(State v);
    void setProgress(qreal v);


private:
    QUrl m_source;
    State m_state;
    qreal m_progress;

private:
    typedef QCache<QUrl, QIODevice> Cache;
    static Cache c_cache;
};

VOLCANO_INLINE const QUrl &Resource::source(void) const
{
    return m_source;
}

VOLCANO_INLINE Resource::State Resource::state(void) const
{
    return m_state;
}

VOLCANO_INLINE void Resource::setState(State v)
{
    if (m_state != v)
    {
        m_state = v;
        stateChanged();
    }
}

VOLCANO_INLINE qreal Resource::progress(void) const
{
    return m_progress;
}

VOLCANO_INLINE void Resource::setProgress(qreal v)
{
    Q_ASSERT(0.0f <= v && v <= 1.0f);

    if (qFuzzyCompare(m_progress, v))
        return;

    m_progress = v;
    progressChanged();
}

VOLCANO_INLINE QUrl Resource::baseUrl(void) const
{
    return qmlEngine(this)->baseUrl();
}

VOLCANO_END

#endif // VOLCANO_RESOURCE_HPP
