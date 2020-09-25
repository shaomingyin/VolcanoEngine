//
//
#include <Volcano/Game/Model.hpp>

VOLCANO_GAME_BEGIN

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

VOLCANO_GAME_END
