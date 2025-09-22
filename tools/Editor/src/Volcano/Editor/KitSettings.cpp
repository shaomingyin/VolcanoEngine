//
//
#include <projectexplorer/projectexplorerconstants.h>

#include <Volcano/Editor/KitSettings.h>

VOLCANO_EDITOR_BEGIN

KitSettingsWidget::KitSettingsWidget()
    : add_button_("Add...")
    , clone_button_("Clone")
    , remove_button_("Remove") {
    actions_layout_.addWidget(&add_button_);
    actions_layout_.addWidget(&clone_button_);
    actions_layout_.addWidget(&remove_button_);
    main_layout_.addWidget(&kit_tree_);
    main_layout_.addLayout(&actions_layout_);
    setLayout(&main_layout_);
    setOnApply([this] { onApply(); });
    setOnFinish([this] { onFinish(); });
    setOnCancel([this] { onCancel(); });
}

void KitSettingsWidget::onApply() {

}

void KitSettingsWidget::onFinish() {

}

void KitSettingsWidget::onCancel() {

}

KitSettingsPage::KitSettingsPage() {
    setDisplayName("VolcanoEngine");
    setCategory(ProjectExplorer::Constants::KITS_SETTINGS_CATEGORY);
    setId("volcanokitsettings");
    setWidgetCreator([] {
        return new KitSettingsWidget();
    });
}

VOLCANO_EDITOR_END
