//
//
#ifndef VOLCANO_DIRECTIONALLIGHT_HPP
#define VOLCANO_DIRECTIONALLIGHT_HPP

#include <QColor>

#include <Volcano/Common.hpp>
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

class DirectionalLight: public Light
{
    Q_OBJECT

public:
    DirectionalLight(Node *parent = nullptr);
    ~DirectionalLight(void) override;
};

VOLCANO_END

#endif // VOLCANO_DIRECTIONALLIGHT_HPP
