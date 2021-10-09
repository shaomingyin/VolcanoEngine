//
//
#ifndef VOLCANO_GAME_GRAPHICS_RARENDERER_HPP
#define VOLCANO_GAME_GRAPHICS_RARENDERER_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderer: public QObject {
    Q_OBJECT

public:
    Renderer(QObject *parent = nullptr);
    ~Renderer(void) override;

public:
    bool init(void);
    void resize(int width, int height);
    void beginFrame(void);
    void endFrame(void);


private:
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
