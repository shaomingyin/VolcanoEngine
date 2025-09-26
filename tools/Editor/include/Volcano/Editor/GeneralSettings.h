//
//
#ifndef VOLCANO_EDITOR_GENERALSETTINGS_H
#define VOLCANO_EDITOR_GENERALSETTINGS_H

#include <utils/aspects.h>
#include <coreplugin/dialogs/ioptionspage.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class GeneralSettings: public Utils::AspectContainer {
    Q_OBJECT

public:
    GeneralSettings();

private:
    Utils::FilePathAspect test_;
};

class GeneralSettingsPage: public Core::IOptionsPage {
public:
    GeneralSettingsPage();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_GENERALSETTINGS_H
