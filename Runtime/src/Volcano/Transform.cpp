//
//
#include <Volcano/Transform.hpp>

VOLCANO_BEGIN

Transform Transform::identity_(
    { 0.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f, 0.0f }
);

VOLCANO_END
