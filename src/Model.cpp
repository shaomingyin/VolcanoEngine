//
//
#include <QRunnable>

#include <Volcano/OpenGL.hpp>
#include <Volcano/Model.hpp>

VOLCANO_BEGIN

Model::Model(QObject *parent):
    Resource(parent)
{
}

Model::~Model(void)
{
    release();
}

void Model::onSourceChanged(const QUrl &url)
{
    release();


}

void Model::release(void)
{

}

VOLCANO_END
