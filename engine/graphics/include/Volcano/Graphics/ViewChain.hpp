//
//
#ifndef VOLCANO_GRAPHICS_VIEWCHAIN_HPP
#define VOLCANO_GRAPHICS_VIEWCHAIN_HPP

#include <vector>

#include <QAtomicInt>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

class ViewChain {
    Q_DISABLE_COPY_MOVE(ViewChain)

public:
    ViewChain(qsizetype len = 2);
    virtual ~ViewChain(void);

public:
    qsizetype length(void) const;
    View *acquire(void);
    void commit(View *view);
    void consume(void);

private:
    using ViewList = std::vector<View *>;

private:
    QAtomicInt m_state;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEWCHAIN_HPP
