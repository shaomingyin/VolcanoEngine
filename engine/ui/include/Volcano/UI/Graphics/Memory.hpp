//
//
#ifndef VOLCANO_UI_GRPHICS_MEMORY_HPP
#define VOLCANO_UI_GRPHICS_MEMORY_HPP

#include <QList>
#include <QObject>

#include <Volcano/UI/Graphics/Common.hpp>
#include <Volcano/UI/Graphics/Heap.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class VOLCANO_UI_API Memory: public QObject {
    Q_OBJECT

public:
    Memory(QObject *parent = nullptr);
    ~Memory(void) override;

public:
    Buffer *allocStaticVertices(int count);
    Buffer *allocStaticVertexIndices(int count);
    Buffer *allocDynamicVertices(int count);
    Buffer *allocDynamicVertexIndices(int count);

protected:
    using HeapList = QList<Heap *>;

    Buffer *allocBuffer(HeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type  type, QOpenGLBuffer::UsagePattern usage);

private:
    HeapList m_staticVertexHeapList;
    HeapList m_staticVertexIndexHeapList;
    HeapList m_dynamicVertexHeapList;
    HeapList m_dynamicVertexIndexHeapList;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_MEMORY_HPP
