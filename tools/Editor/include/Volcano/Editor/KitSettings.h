//
//
#ifndef VOLCANO_EDITOR_KITSETTINGS_H
#define VOLCANO_EDITOR_KITSETTINGS_H

#include <QBoxLayout>
#include <QTreeWidget>
#include <QPushButton>

#include <coreplugin/dialogs/ioptionspage.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class KitSettingsWidget: public Core::IOptionsPageWidget {
    Q_OBJECT

public:
    KitSettingsWidget();

private:
    void onApply();
    void onFinish();
    void onCancel();

private:
    QHBoxLayout main_layout_;
    QTreeWidget kit_tree_;
    QVBoxLayout actions_layout_;
    QPushButton add_button_;
    QPushButton clone_button_;
    QPushButton remove_button_;
};

class KitSettingsPage: public Core::IOptionsPage {
public:
    KitSettingsPage();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_KITSETTINGS_H
