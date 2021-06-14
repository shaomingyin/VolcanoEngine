//
//
#ifndef VOLCANO_SPOTLIGHT_HPP
#define VOLCANO_SPOTLIGHT_HPP

#include <Volcano/Common.hpp>
#include <Volcano/DirectionalLight.hpp>

VOLCANO_BEGIN

class SpotLight: public DirectionalLight {
    Q_OBJECT

public:
    SpotLight(QObject *parent = nullptr);
    ~SpotLight(void) override;
};

VOLCANO_END

#endif // VOLCANO_SPOTLIGHT_HPP
