//
//
#include <Volcano/Object.hpp>

VOLCANO_BEGIN

Object::Object(QObject *parent):
    QObject(parent)
{
}

Object::~Object(void)
{
}

bool Object::isEnabled(void) const
{
    return m_isEnabled;
}

void Object::setEnabled(bool v)
{
    if (m_isEnabled != v) {
        m_isEnabled = v;
        emit enabledChanged(v);
    }
}

VOLCANO_END
