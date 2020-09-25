//
//
#include <Volcano/Model.hpp>
#include <Volcano/ResourceManager.hpp>

VOLCANO_BEGIN

Model::Model(QObject *parent):
    Component(parent)
{
}

Model::~Model(void)
{
}

void Model::setSource(const QUrl &r)
{
    if (m_source == r)
        return;

    m_source = r;
    startLoading();

    sourceChanged(r);
}

VOLCANO_END
