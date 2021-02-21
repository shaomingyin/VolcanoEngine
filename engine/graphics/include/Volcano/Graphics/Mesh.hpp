//
//
#ifndef VOLCANO_GRPHICS_MESH_HPP
#define VOLCANO_GRPHICS_MESH_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Memory.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_GRAPHICS_API Mesh: public QObject {
    Q_OBJECT

public:
    Mesh(Memory &memory, QObject *parent = nullptr);
    ~Mesh(void) override;

private:
    Memory &m_memory;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_MESH_HPP
