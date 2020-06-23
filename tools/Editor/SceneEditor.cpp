//
//
#include <Volcano/Editor/SceneEditor.h>

VOLCANO_EDITOR_BEGIN

SceneEditor::SceneEditor(void):
    m_glWidget(nullptr)
{
}

SceneEditor::~SceneEditor(void)
{
    if (m_glWidget != nullptr)
        delete m_glWidget;
}

bool SceneEditor::init(void)
{
    Q_ASSERT(m_glWidget == nullptr);
    m_glWidget = new QOpenGLWidget();
    return true;
}

Core::IDocument *SceneEditor::document(void) const
{
    return nullptr;
}

QWidget *SceneEditor::toolBar(void)
{
    return nullptr;
}

VOLCANO_EDITOR_END
