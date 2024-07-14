//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <QSize>
#include <QObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_GRAPHICS_API Renderer: public QObject {
    Q_OBJECT

public:
    Renderer(QObject* parent = nullptr);

public:
    bool init(int width, int height);
    bool isInited() const;

    const QSize size() const {
        return size_;
    }

protected:
    virtual bool doInit(int width, int height);

private:
    bool is_inited_;
    QSize size_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
