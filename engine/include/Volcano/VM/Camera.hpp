//
//
#ifndef VOLCANO_VM_CAMERA_HPP
#define VOLCANO_VM_CAMERA_HPP

#include <Volcano/Graphics/View.hpp>
#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Object.hpp>

VOLCANO_VM_BEGIN

class Camera: public Object {
public:
    Camera(void);
    ~Camera(void) override;

public:
};

VOLCANO_VM_END

#endif // VOLCANO_VM_CAMERA_HPP
