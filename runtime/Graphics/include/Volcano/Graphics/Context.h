//
//
#ifndef VOLCANO_GRAPHICS_CONTEXT_H
#define VOLCANO_GRAPHICS_CONTEXT_H

#include <QThread>
#include <QObject>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Buffer.h>

VOLCANO_GRAPHICS_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject* parent = nullptr);
    virtual ~Context() = default;

public:
    virtual Buffer* allocStaticVertexBuffer(GLsizei size) = 0;
    virtual Buffer* allocStaticIndexBuffer(GLsizei size) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CONTEXT_H
