//
//
#ifndef VOLCANO_GAME_SPOTLIGHT_HPP
#define VOLCANO_GAME_SPOTLIGHT_HPP

#include <QVector3D>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/DirectionalLight.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API SpotLight: public DirectionalLight {
    Q_OBJECT

public:
    SpotLight(QObject* parent = nullptr);
    SpotLight(Context& context, QObject* parent = nullptr);

public:

signals:


private:

};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SPOTLIGHT_HPP
