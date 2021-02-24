//
//
#ifndef VOLCANO_UI_GRPHICS_MESH_HPP
#define VOLCANO_UI_GRPHICS_MESH_HPP

#include <QObject>

#include <Volcano/UI/Graphics/Common.hpp>
#include <Volcano/UI/Graphics/Memory.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class VOLCANO_UI_API Mesh: public QObject {
    Q_OBJECT

public:
    Mesh(Memory &memory, QObject *parent = nullptr);
    ~Mesh(void) override;

private:
    Memory &m_memory;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_MESH_HPP
