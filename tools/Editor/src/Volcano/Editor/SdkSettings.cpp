//
//
#include <projectexplorer/projectexplorerconstants.h>

#include <Volcano/Editor/SdkSettings.h>

VOLCANO_EDITOR_BEGIN

SdkSettings::SdkSettings(Context& context)
    : context_(context) {
}

SdkSettingsPage::SdkSettingsPage(Context& context) {
    setDisplayName("SDK");
    setCategory(SETTINGS_CATEGORY);
    setId(SDK_SETTINGS_PAGE);
    setSettingsProvider([&context] {
        return new SdkSettings(context);
    });
}

VOLCANO_EDITOR_END
