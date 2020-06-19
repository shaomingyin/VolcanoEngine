//
//
#ifndef VOLCANO_EDITOR_SCENEEDITOR_H
#define VOLCANO_EDITOR_SCENEEDITOR_H

#include <QOpenGLWidget>

#include <coreplugin/idocument.h>
#include <coreplugin/editormanager/ieditor.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class SceneEditor: public Core::IEditor
{
    Q_OBJECT

public:
    SceneEditor(void);
    ~SceneEditor(void) override;

public:
    bool init(void);
    Core::IDocument *document(void) const override;
    QWidget *toolBar(void) override;

private:
    QOpenGLWidget *m_glWidget;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEEDITOR_H
