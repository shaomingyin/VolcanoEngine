//
//
#ifndef VOLCANO_SCENE_MODEL_HPP
#define VOLCANO_SCENE_MODEL_HPP

#include <QVector>

#include <Volcano/Resource.hpp>
#include <Volcano/Scene/Common.hpp>
#include <Volcano/Scene/Component.hpp>

VOLCANO_SCENE_BEGIN

class VOLCANO_API Model: public Component
{
    Q_OBJECT

public:
    Model(QObject *parent = nullptr);
    ~Model(void) override;

private:
    void release(void);

private:
};

VOLCANO_SCENE_END

#endif // VOLCANO_SCENE_MODEL_HPP
