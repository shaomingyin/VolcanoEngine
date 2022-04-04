//
//
#ifndef VOLCANO_EDITOR_GAMEOBJECTPROPERTYVIEW_HPP
#define VOLCANO_EDITOR_GAMEOBJECTPROPERTYVIEW_HPP

#include <QTreeWidget>

#include <projectexplorer/projectnodes.h>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/GameObjectNode.hpp>

VOLCANO_EDITOR_BEGIN

class GameObjectPropertyView: public QTreeWidget {
    Q_OBJECT

public:
    GameObjectPropertyView(QWidget *parent = nullptr);
    ~GameObjectPropertyView(void) override;

protected:
    void updateTree(void);

protected slots:
    void onCurrentNodeChanged(ProjectExplorer::Node *m_node);

private:
    GameObjectNode *m_node;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_GAMEOBJECTPROPERTYVIEW_HPP
