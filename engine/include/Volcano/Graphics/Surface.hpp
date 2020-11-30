//
//
#ifndef VOLCANO_GRAPHICS_SURFACE_HPP
#define VOLCANO_GRAPHICS_SURFACE_HPP

#include <Volcano/Size.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Surface {
public:
    Surface(void);
    virtual ~Surface(void);

public:
    virtual bool isValid(void);
    const SizeI &size(void) const;
	int width(void) const;
	int height(void) const;
    void resize(const SizeI &r);
	virtual void resize(int width, int height);
    virtual bool activate(void);
    virtual void deactivate(void);
    bool isActivated(void) const;

private:
    SizeI m_size;
    bool m_activated;
};

VOLCANO_INLINE const SizeI &Surface::size(void) const
{
	return m_size;
}

VOLCANO_INLINE int Surface::width(void) const
{
    return m_size.width();
}

VOLCANO_INLINE int Surface::height(void) const
{
    return m_size.height();
}

VOLCANO_INLINE void Surface::resize(const SizeI &r)
{
    resize(r.width(), r.height());
}

VOLCANO_INLINE bool Surface::isActivated(void) const
{
    return m_activated;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SURFACE_HPP
