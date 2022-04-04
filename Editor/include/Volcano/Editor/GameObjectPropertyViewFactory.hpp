//
//
#ifndef VOLCANO_EDITOR_GAMEOBJECTPROPERTYVIEWFACTORY_HPP
#define VOLCANO_EDITOR_GAMEOBJECTPROPERTYVIEWFACTORY_HPP

#include <coreplugin/inavigationwidgetfactory.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class GameObjectPropertyViewFactory: public Core::INavigationWidgetFactory {
public:
    GameObjectPropertyViewFactory(void);

public:
    Core::NavigationView createWidget(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_GAMEOBJECTPROPERTYVIEWFACTORY_HPP
