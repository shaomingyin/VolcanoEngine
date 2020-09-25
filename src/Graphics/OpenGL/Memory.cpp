//
//
#include <Volcano/Graphics/OpenGL/Memory.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

static const int heapOrder = 26;
static const int heapSize = 1 << heapOrder;

Memory::Memory(void)
{
}

Memory::~Memory(void)
{
}

Buffer *Memory::allocBuffer(int size, HeapPtrList &list, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage)
{
    Q_ASSERT(size > 0);

    if (size > heapSize)
        return nullptr;

    for (auto it(list.begin()); it != list.end(); ++it)
    {
        Buffer *buf = (*it)->allocBuffer(size);
        if (buf != nullptr)
            return buf;
    }

    HeapPtr heap(new Heap(type, usage));
    if (!heap || !heap->init(heapSize))
        return nullptr;

    list.prepend(heap);

    return heap->allocBuffer(size);
}

VOLCANO_GRAPHICS_OPENGL_END
