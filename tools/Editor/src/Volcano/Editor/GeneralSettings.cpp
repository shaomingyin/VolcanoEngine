//
//
#include <QLabel>
#include <QBoxLayout>
#include <QTreeWidget>
#include <QPushButton>

#include <utils/layoutbuilder.h>

#include <Volcano/Editor/GeneralSettings.h>

VOLCANO_EDITOR_BEGIN

// GeneralSettings

GeneralSettings::GeneralSettings() {
    setAutoApply(false);

    test_.setDefaultValue("defaultval");
    setLayouter([this] {
        return Layouting::Column {
            Layouting::Row { new QLabel("test"), test_ }
        };
    });
}

// GeneralSettingsPage

GeneralSettingsPage::GeneralSettingsPage() {
    setCategory(SETTINGS_CATEGORY_ID);
    setDisplayName("General");
    setId("VolcanoGeneralSettings");
    setSettingsProvider([] {
        return new GeneralSettings();
    });
}

VOLCANO_EDITOR_END
