//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <QSize>
#include <QObject>

#include <Volcano/View.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderer: public View {
    Q_OBJECT

public:
    Renderer(QObject* parent = nullptr);

public:
    bool init();
    bool beginFrame() override;
    void commitFrame() override;
    void cancelFrame() override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
