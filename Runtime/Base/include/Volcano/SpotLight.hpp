//
//
#ifndef VOLCANO_SPOTLIGHT_HPP
#define VOLCANO_SPOTLIGHT_HPP

#include <QVector3D>

#include <Volcano/Common.hpp>
#include <Volcano/DirectionalLight.hpp>

VOLCANO_BEGIN

class SpotLight: public DirectionalLight {
    Q_OBJECT

public:
    SpotLight(QObject* parent = nullptr);
    SpotLight(Context& context, QObject* parent = nullptr);

public:

signals:


private:

};

VOLCANO_END

#endif // VOLCANO_SPOTLIGHT_HPP
