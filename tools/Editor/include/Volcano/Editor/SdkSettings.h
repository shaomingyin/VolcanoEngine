//
//
#ifndef VOLCANO_EDITOR_SDKSETTINGS_H
#define VOLCANO_EDITOR_SDKSETTINGS_H

#include <utils/aspects.h>
#include <coreplugin/dialogs/ioptionspage.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Context.h>

VOLCANO_EDITOR_BEGIN

class SdkSettings: public Utils::AspectContainer {
    Q_OBJECT

public:
    SdkSettings(Context& context);

private:
    Context& context_;
};

class SdkSettingsPage: public Core::IOptionsPage {
public:
    SdkSettingsPage(Context& context);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SDKSETTINGS_H
