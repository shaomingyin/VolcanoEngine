//
//
#include <Volcano/Editor/GameObjectPropertyView.hpp>
#include <Volcano/Editor/GameObjectPropertyViewFactory.hpp>

VOLCANO_EDITOR_BEGIN

GameObjectPropertyViewFactory::GameObjectPropertyViewFactory(void)
{
    setId("GameObjectPropertyView");
    setDisplayName("Properties");
    //setActivationSequence(QKeySequence(useMacShortcuts ? OpenEditorsWidget::tr("Meta+O")
    //                                                   : OpenEditorsWidget::tr("Alt+O")));
    //setPriority(200);
}

Core::NavigationView GameObjectPropertyViewFactory::createWidget(void)
{
    return { new GameObjectPropertyView, { } };
}

VOLCANO_EDITOR_END
