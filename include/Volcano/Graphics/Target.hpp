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
    Target(int width, int height);
    virtual ~Target(void);

public:
    const QSize &size(void) const;
    void resize(const QSize &r);
    void resize(int width, int height);

protected:
    virtual void onSizeChanged(void) = 0;

private:
    QSize m_size;
};

VOLCANO_INLINE const QSize &Target::size(void) const
{
    return m_size;
}

VOLCANO_INLINE void Target::resize(const QSize &r)
{
    if (m_size != r)
    {
        m_size = r;
        onSizeChanged();
    }
}

VOLCANO_INLINE void Target::resize(int width, int height)
{
    resize(QSize(width, height));
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TARGET_HPP
