//
//
#ifndef VOLCANO_GRAPHICS_TARGET_HPP
#define VOLCANO_GRAPHICS_TARGET_HPP

#include <QSize>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Target
{
public:
    Target(void);
    virtual ~Target(void);

public:
    const QSize &size(void) const;
    void resize(const QSize &r);

private:
    QSize m_size;
};

VOLCANO_INLINE const QSize &Target::size(void) const
{
    return m_size;
}

VOLCANO_INLINE void Target::resize(const QSize &r)
{
    m_size = r;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TARGET_HPP
