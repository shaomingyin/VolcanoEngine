//
//
#include <QOpenGLContext>

#include <Volcano/Graphics/ViewChain.hpp>

VOLCANO_GRAPHICS_BEGIN

ViewChain::ViewChain(qsizetype len)
{
}

ViewChain::~ViewChain(void)
{
}

qsizetype ViewChain::length(void) const
{
    return 0;
}

View *ViewChain::acquire(void)
{
    return nullptr;
}

void ViewChain::commit(View *view)
{
}

void ViewChain::consume(void)
{
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);

    // TODO
}

VOLCANO_GRAPHICS_END
