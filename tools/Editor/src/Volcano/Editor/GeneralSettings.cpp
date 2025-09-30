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

GeneralSettings::GeneralSettings(Context& context)
    : context_(context) {
    setAutoApply(false);

    test_.setDefaultValue("defaultval");
    setLayouter([this] {
        return Layouting::Column {
            Layouting::Row { new QLabel("test"), test_ }
        };
    });
}

// GeneralSettingsPage

GeneralSettingsPage::GeneralSettingsPage(Context& context) {
    setCategory(SETTINGS_CATEGORY);
    setDisplayName("General");
    setId(GENERAL_SETTINGS_PAGE);
    setSettingsProvider([&context] {
        return new GeneralSettings(context);
    });
}

VOLCANO_EDITOR_END
