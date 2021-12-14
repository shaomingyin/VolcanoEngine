//
//
#ifndef VOLCANO_GAME_LOADABLECOMPONENT_HPP
#define VOLCANO_GAME_LOADABLECOMPONENT_HPP

#include <QUrl>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class LoadableComponent: public Component {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
    LoadableComponent(QObject *parent = nullptr);
    ~LoadableComponent(void) override;

public:
    QUrl source(void) const;
    void setSource(QUrl v);

signals:
    void sourceChanged(const QUrl &v);

private:
    QUrl m_source;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LOADABLECOMPONENT_HPP
