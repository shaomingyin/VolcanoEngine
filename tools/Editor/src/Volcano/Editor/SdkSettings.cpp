//
//
#include <QLabel>
#include <QBoxLayout>
#include <QTreeWidget>
#include <QPushButton>

#include <utils/layoutbuilder.h>
#include <projectexplorer/projectexplorerconstants.h>

#include <Volcano/Editor/SdkSettings.h>

VOLCANO_EDITOR_BEGIN

// SdkSettings

SdkSettings::SdkSettings() {
    setAutoApply(false);

    test_.setDefaultValue("defaultval");
    setLayouter([this] {
        return Layouting::Column {
            Layouting::Row { new QLabel("test"), test_ }
        };
    });
}

// SdkSettingsPage

SdkSettingsPage::SdkSettingsPage() {
    setCategory(ProjectExplorer::Constants::SDK_SETTINGS_CATEGORY);
    setDisplayName("VolcanoEngine");
    setId("VolcanoSdkSettings");
    setSettingsProvider([] {
        return new SdkSettings();
    });
}

VOLCANO_EDITOR_END
