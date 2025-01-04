//
//
#include <Volcano/Resource/Model.h>

VOLCANO_RESOURCE_BEGIN

QDataStream &operator<<(QDataStream &s, const Model& v) {
    s << v.mesh_list;
    s << v.texture_list;
    s << v.material_list;
    return s;
}

QDataStream &operator>>(QDataStream &s, Model& v) {
    s >> v.mesh_list;
    s >> v.texture_list;
    s >> v.material_list;
    return s;
}

VOLCANO_RESOURCE_END
