//
//
#include <Volcano/Editor/CameraView.hpp>

VOLCANO_EDITOR_BEGIN

CameraView::CameraView(QWidget *parent):
    QQuickWidget(parent)
{
}

CameraView::~CameraView(void)
{
}

bool CameraView::init(void)
{
    return true;
}

VOLCANO_EDITOR_END
