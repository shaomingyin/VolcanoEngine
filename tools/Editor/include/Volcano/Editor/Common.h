//
//
#ifndef VOLCANO_EDITOR_COMMON_H
#define VOLCANO_EDITOR_COMMON_H

#include <Volcano/Common.h>
#include <Volcano/Editor/Config.h>

#define VOLCANO_EDITOR_BEGIN VOLCANO_BEGIN namespace Editor {
#define VOLCANO_EDITOR_END } VOLCANO_END

VOLCANO_EDITOR_BEGIN

const char SETTINGS_CATEGORY[] = "Volcano.Editor.SettingsCategory";

const char GENERAL_SETTINGS_PAGE[] = "Volcano.Editor.GeneralSettings";
const char SDK_SETTINGS_PAGE[] = "Volcano.Editor.SdkSettingsPage";

const char KIT_ASPECT[] = "Volcano.Editor.KitAspect";
const char SDK[] = "Volcano.Editor.Sdk";

const char SCENE_FILE_MIMETYPE[] = VOLCANO_EDITOR_MIMETYPE_SCENE_FILE;

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_COMMON_H
