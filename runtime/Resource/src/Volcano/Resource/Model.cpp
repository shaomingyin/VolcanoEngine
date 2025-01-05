//
//
#include <Volcano/Resource/Model.h>

VOLCANO_RESOURCE_BEGIN

static constexpr quint32 MAGIC = 0x20120920;

QDataStream &operator<<(QDataStream &s, const Model& v) {
    s << MAGIC;
    s << version;
    s << v.mesh_list;
    s << v.texture_list;
    s << v.material_list;
    return s;
}

QDataStream &operator>>(QDataStream &s, Model& v) {
    quint32 magic = 0;
    s >> magic;
    if (magic != MAGIC) {
        throw std::runtime_error("Invalid model magic");
    }
    QVersionNumber ver;
    s >> ver;
    if (ver > version) {
        throw std::runtime_error("New version model is not supported.");
    }
    s >> v.mesh_list;
    s >> v.texture_list;
    s >> v.material_list;
    return s;
}

VOLCANO_RESOURCE_END
