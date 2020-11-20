//
//
#ifndef VOLCANO_GRAPHICS_HEAP_HPP
#define VOLCANO_GRAPHICS_HEAP_HPP

#include <list>

#include <GL/Buffer.hpp>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Buffer.hpp>

VOLCANO_GRAPHICS_BEGIN

class Heap {
public:
    Heap(GLenum target, GLenum usage);
    virtual ~Heap(void);

public:
    bool init(int order = 26);
    void release(void);
    const GL::Buffer &glBuffer(void) const;
    GLsizeiptr freeSize(void) const;
    Buffer *allocBuffer(GLsizeiptr size);

protected:
    void freeBuffer(Buffer *buf);
    void bindBuffer(Buffer *buf);
    void *mapBuffer(Buffer *buf);
    void unmapBuffer(Buffer *buf);

private:
    typedef std::list<Buffer *> BufferList;

private:
    GL::Buffer m_glBuffer;
    GLsizeiptr m_freeSize;
    void *m_map;
    int m_mapCount;
    BufferList m_bufferList;
};

VOLCANO_INLINE const GL::Buffer &Heap::glBuffer(void) const
{
    return m_glBuffer;
}

VOLCANO_INLINE GLsizeiptr Heap::freeSize(void) const
{
    return m_freeSize;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_HEAP_HPP
