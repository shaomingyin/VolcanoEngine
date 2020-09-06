//
//
#include <QRunnable>

#include <Volcano/Scene/Model.hpp>

VOLCANO_SCENE_BEGIN

Model::Model(QObject *parent)
{
}

Model::~Model(void)
{
    release();
}

void Model::release(void)
{

}

VOLCANO_SCENE_END
