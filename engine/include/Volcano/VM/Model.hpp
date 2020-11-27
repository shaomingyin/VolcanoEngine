//
//
#ifndef VOLCANO_VM_MODEL_HPP
#define VOLCANO_VM_MODEL_HPP

#include <Volcano/Graphics/Mesh.hpp>

#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Component.hpp>

VOLCANO_VM_BEGIN

class Model: public Component {
public:
    Model(void);
    ~Model(void) override;

public:
    void applyToView(Graphics::View &view) override;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_MODEL_HPP
