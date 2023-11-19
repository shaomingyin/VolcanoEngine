//
//
#ifndef VOLCANO_GRAPHICS_CONTEXT_HPP
#define VOLCANO_GRAPHICS_CONTEXT_HPP

#include <QSize>
#include <QObject>

#include <Volcano/Game/World.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject* parent = nullptr);
    ~Context(void) override;

public:
    virtual bool Init(const QSize& size);
    virtual void Update(Duration elapsed);
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CONTEXT_HPP
