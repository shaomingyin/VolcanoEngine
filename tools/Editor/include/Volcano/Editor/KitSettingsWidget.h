//
//
#ifndef VOLCANO_EDITOR_KITSETTINGSWIDGET_H
#define VOLCANO_EDITOR_KITSETTINGSWIDGET_H

#include <QVBoxLayout>

#include <coreplugin/dialogs/ioptionspage.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class KitSettingsWidget: public Core::IOptionsPageWidget {
    Q_OBJECT

public:
    KitSettingsWidget();

private:
    QVBoxLayout main_layout_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_KITSETTINGSWIDGET_H
