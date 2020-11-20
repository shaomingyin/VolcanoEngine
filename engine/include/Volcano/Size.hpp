//
//
#ifndef VOLCANO_SIZE_HPP
#define VOLCANO_SIZE_HPP

#include <type_traits>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

template <typename T>
class Size {
public:
    VOLCANO_INLINE Size(void):
        m_width(0),
        m_height(0)
    {
    }

    VOLCANO_INLINE Size(const Size &that):
        m_width(that.m_width),
        m_height(that.m_height)
    {
    }

    VOLCANO_INLINE Size(T width, T height):
        m_width(width),
        m_height(height)
    {
        VOLCANO_ASSERT(width >= 0);
        VOLCANO_ASSERT(height >= 0);
    }

public:
    VOLCANO_INLINE bool isEmpty(void) const
    {
        return (m_width < 1 || m_height < 1);
    }

    VOLCANO_INLINE int width(void) const
    {
        return m_width;
    }

    VOLCANO_INLINE void setWidth(int v)
    {
        VOLCANO_ASSERT(v >= 0);

        m_width = v;
    }

    VOLCANO_INLINE int height(void) const
    {
        return m_height;
    }

    VOLCANO_INLINE void setHeight(int v)
    {
        VOLCANO_ASSERT(v >= 0);

        m_height = v;
    }

    VOLCANO_INLINE void set(int width, int height)
    {
        VOLCANO_ASSERT(width >= 0);
        VOLCANO_ASSERT(height >= 0);

        m_width = width;
        m_height = height;
    }

    VOLCANO_INLINE Size &operator=(const Size &that)
    {
        m_width = that.m_width;
        m_height = that.m_height;

        return *this;
    }

private:
    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "type of 'size_template' must be a sclar.");

    T m_width;
    T m_height;
};

using SizeI = Size<int>;
using SizeL = Size<long>;
using SizeF = Size<float>;
using SizeD = Size<double>;

VOLCANO_END

#endif // VOLCANO_SIZE_HPP
