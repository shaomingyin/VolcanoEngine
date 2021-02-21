//
//
#ifndef VOLCANO_GAME_PLAY_HPP
#define VOLCANO_GAME_PLAY_HPP

#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/PlayBase.hpp>

VOLCANO_GAME_BEGIN

class Play: public QObject, public PlayBase {
    Q_OBJECT
    Q_INTERFACES(Volcano::Game::PlayBase)
    Q_PLUGIN_METADATA(IID VOLCANO_GAME_PLAY_IID)

public:
    Play(QObject *parent = nullptr);
    ~Play(void) override;

public:
    bool init(Context *context) override;
    void shutdown(void) override;
    void update(float elapsed) override;
    Context *context(void);

private:
    Context *m_context;
};

VOLCANO_INLINE Context *Play::context(void)
{
    return m_context;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_PLAY_HPP
