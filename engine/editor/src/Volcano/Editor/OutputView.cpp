//
//
#include <Volcano/Editor/OutputView.hpp>

VOLCANO_EDITOR_BEGIN

OutputView::OutputView(QWidget *parent):
    QTextEdit(parent)
{
    setReadOnly(true);
}

OutputView::~OutputView(void)
{
}

VOLCANO_EDITOR_END
