//
//
#include <projectexplorer/projectexplorerconstants.h>

#include <Volcano/Editor/KitSettingsWidget.h>
#include <Volcano/Editor/KitSettingsPage.h>

VOLCANO_EDITOR_BEGIN

KitSettingsPage::KitSettingsPage() {
    setDisplayName("VolcanoEngine");
    setCategory(ProjectExplorer::Constants::KITS_SETTINGS_CATEGORY);
    setId("volcanokitsettings");
    setWidgetCreator([] {
        return new KitSettingsWidget();
    });
}

VOLCANO_EDITOR_END
