//
//
#include <QString>
#include <QStringList>

#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>

#include <Volcano/Game/World.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Mesh.hpp>

#include <Volcano/Editor/GameObjectPropertyView.hpp>

VOLCANO_EDITOR_BEGIN

GameObjectPropertyView::GameObjectPropertyView(QWidget *parent):
    QTreeWidget(parent),
    m_node(nullptr)
{
    onCurrentNodeChanged(ProjectExplorer::ProjectTree::instance()->currentNode());
    connect(ProjectExplorer::ProjectTree::instance(), &ProjectExplorer::ProjectTree::currentNodeChanged,
            this, &GameObjectPropertyView::onCurrentNodeChanged);
}

GameObjectPropertyView::~GameObjectPropertyView(void)
{
}

void GameObjectPropertyView::onCurrentNodeChanged(ProjectExplorer::Node *node)
{
    qDebug() << __FUNCTION__;

    if (m_node == node)
        return;

    auto gameObjectNode = dynamic_cast<GameObjectNode *>(node);
    if (!gameObjectNode)
        return;

    m_node = gameObjectNode;
    updateTree();
}

void GameObjectPropertyView::updateTree(void)
{
    clear();

    if (m_node == nullptr)
        return;

    auto gameObject = m_node->gameObject();
    Q_ASSERT(gameObject != nullptr);

    auto metaObject = gameObject->metaObject();
    int n = metaObject->propertyCount();
    for (int i = 0; i < n; ++i) {
        addTopLevelItem(new QTreeWidgetItem(this, { QLatin1String(metaObject->property(i).name()) }));
    }
}

VOLCANO_EDITOR_END
