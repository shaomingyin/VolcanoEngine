//
//
#include <Volcano/Editor/ProjectView.hpp>

VOLCANO_EDITOR_BEGIN

ProjectView::ProjectView(QWidget *parent):
	QListView(parent)
{
}

ProjectView::~ProjectView(void)
{
}

bool ProjectView::init(void)
{
	setModel(&m_model);

	return true;
}

VOLCANO_EDITOR_END
