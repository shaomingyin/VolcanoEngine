//
//
#ifndef VOLCANO_EDITOR_KITSETTINGS_H
#define VOLCANO_EDITOR_KITSETTINGS_H

#include <utils/aspects.h>
#include <coreplugin/dialogs/ioptionspage.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class KitSettings: public Utils::AspectContainer {
    Q_OBJECT

public:
    KitSettings();

private:
};

class KitSettingsPage: public Core::IOptionsPage {
public:
    KitSettingsPage();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_KITSETTINGS_H
