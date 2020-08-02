//
//
#ifndef VOLCANO_SPOTLIGHT_HPP
#define VOLCANO_SPOTLIGHT_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

class VOLCANO_API SpotLight: public Light
{
    Q_OBJECT

public:
    SpotLight(Node *parent = nullptr);
    ~SpotLight(void) override;
};

VOLCANO_END

#endif // VOLCANO_SPOTLIGHT_HPP
