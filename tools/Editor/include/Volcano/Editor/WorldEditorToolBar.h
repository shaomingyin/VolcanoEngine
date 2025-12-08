//
//
#ifndef VOLCANO_EDITOR_WORLDEDITORTOOLBAR_H
#define VOLCANO_EDITOR_WORLDEDITORTOOLBAR_H

#include <QToolBar>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class WorldEditorToolBar: public QToolBar {
    Q_OBJECT

public:
    WorldEditorToolBar(QWidget* parent = nullptr);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_WORLDEDITORTOOLBAR_H
