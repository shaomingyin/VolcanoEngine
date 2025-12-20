//
//
#ifndef VOLCANO_EDITOR_SCENEEDITORTOOLBAR_H
#define VOLCANO_EDITOR_SCENEEDITORTOOLBAR_H

#include <QToolBar>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class SceneEditorToolBar: public QToolBar {
    Q_OBJECT

public:
    SceneEditorToolBar(QWidget* parent = nullptr);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEEDITORTOOLBAR_H
