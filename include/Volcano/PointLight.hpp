//
//
#ifndef VOLCANO_POINTLIGHT_HPP
#define VOLCANO_POINTLIGHT_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

class PointLight: public Light
{
    Q_OBJECT

public:
    PointLight(Node *parent = nullptr);
    ~PointLight(void) override;
};

VOLCANO_END

#endif // VOLCANO_POINTLIGHT_HPP
