//
//
#ifndef VOLCANO_GAME_MODEL_HPP
#define VOLCANO_GAME_MODEL_HPP

#include <QUrl>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class Model: public Component
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
    Model(QObject *parent = nullptr);
    ~Model(void) override;

public:
    QUrl source(void) const;
    void setSource(const QUrl &r);

signals:
    void sourceChanged(const QUrl &r);

protected:
    virtual void startLoading(void) = 0;

private:
    QUrl m_source;
};

VOLCANO_INLINE QUrl Model::source(void) const
{
    return m_source;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MODEL_HPP
