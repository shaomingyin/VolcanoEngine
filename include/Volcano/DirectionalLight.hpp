//
//
#ifndef VOLCANO_DIRECTIONALLIGHT_HPP
#define VOLCANO_DIRECTIONALLIGHT_HPP

#include <QColor>

#include <Volcano/Common.hpp>
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

class VOLCANO_API DirectionalLight: public Light
{
    Q_OBJECT

public:
    DirectionalLight(QObject *parent = nullptr);
    ~DirectionalLight(void) override;
};

VOLCANO_END

#endif // VOLCANO_DIRECTIONALLIGHT_HPP
