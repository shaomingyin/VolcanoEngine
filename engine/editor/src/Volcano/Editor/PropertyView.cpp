//
//
#include <Volcano/Editor/PropertyView.hpp>

VOLCANO_EDITOR_BEGIN

PropertyView::PropertyView(QWidget *parent):
    QTableView(parent)
{
}

PropertyView::~PropertyView(void)
{
}

QObject *PropertyView::object(void)
{
    return m_object;
}

void PropertyView::setObject(QObject *object)
{
    if (m_object == object)
        return;

    if (m_object != nullptr) {
        // TODO
    }

    update();

    m_object = object;
    emit objectChanged(object);
}

VOLCANO_EDITOR_END
