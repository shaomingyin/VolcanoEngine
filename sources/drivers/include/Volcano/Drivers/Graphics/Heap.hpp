//
//
#ifndef VOLCANO_DRIVERS_GRPHICS_HEAP_HPP
#define VOLCANO_DRIVERS_GRPHICS_HEAP_HPP

#include <list>

#include <GL/Buffer.hpp>

#include <Volcano/Drivers/Graphics/Common.hpp>
#include <Volcano/Drivers/Graphics/Buffer.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class Heap {
public:
    Heap(GLenum target, GLenum usage);
    virtual ~Heap(void);

public:
    bool init(int order = 26);
    void release(void);
    Buffer *allocBuffer(GLsizeiptr size);

private:
    void freeBuffer(Buffer *buf);
    void bindBuffer(Buffer *buf);
    void *mapBuffer(Buffer *buf);
    void unmapBuffer(Buffer *buf);

    private:
    using BufferList = std::list<Buffer *>;

private:
    GL::Buffer m_glBuffer;
    BufferList m_bufferList;
    GLsizeiptr m_freeSize;
    void *m_map;
    int m_mapCount;
};

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRPHICS_HEAP_HPP
