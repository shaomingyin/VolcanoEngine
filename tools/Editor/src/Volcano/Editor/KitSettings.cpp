//
//
#include <projectexplorer/projectexplorerconstants.h>

#include <Volcano/Editor/KitSettings.h>

VOLCANO_EDITOR_BEGIN

KitSettings::KitSettings() {
}

KitSettingsPage::KitSettingsPage() {
    setDisplayName("VolcanoEngine");
    setCategory(ProjectExplorer::Constants::KITS_SETTINGS_CATEGORY);
    setId("volcanokitsettings");
    setSettingsProvider([] {
        return new KitSettings();
    });
}

VOLCANO_EDITOR_END
