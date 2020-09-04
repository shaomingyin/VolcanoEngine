//
//
#ifndef VOLCANO_DRIVERS_GRAPHICS_TARGET_HPP
#define VOLCANO_DRIVERS_GRAPHICS_TARGET_HPP

#include <QSize>
#include <QOpenGLFramebufferObject>

#include <Volcano/Drivers/Graphics/Common.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class Target
{
public:
    Target(void);
    virtual ~Target(void);

public:
    bool init(void);
    const QSize &size(void) const;
    void resize(int width, int height);
    void resize(const QSize &sz);

protected:
    void updateSize(void);

private:
    QSize m_size;
};

VOLCANO_INLINE const QSize &Target::size(void) const
{
    return m_size;
}

VOLCANO_INLINE void Target::resize(int width, int height)
{
    resize(QSize(width, height));
}

VOLCANO_INLINE void Target::resize(const QSize &sz)
{
    if (m_size != sz)
    {
        m_size = sz;
        updateSize();
    }
}

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRAPHICS_TARGET_HPP
