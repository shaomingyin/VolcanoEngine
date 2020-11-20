//
//
#ifndef VOLCANO_RECT_HPP
#define VOLCANO_RECT_HPP

#include <type_traits>

#include <Volcano/Common.hpp>
#include <Volcano/Point.hpp>
#include <Volcano/Size.hpp>

VOLCANO_BEGIN

template <typename T>
class Rect {
public:
    using PointType = Point<T>;
    using SizeType = Size<T>;

public:
    VOLCANO_INLINE Rect(void):
        m_left(0),
        m_top(0),
        m_right(0),
        m_bottom(0)
    {
    }

    VOLCANO_INLINE Rect(const Rect &that):
        m_left(that.m_left),
        m_top(that.m_top),
        m_right(that.m_right),
        m_bottom(that.m_bottom)
    {
    }

    VOLCANO_INLINE Rect(int left, int top, int right, int bottom):
        m_left(left),
        m_top(top),
        m_right(right),
        m_bottom(bottom)
    {
    }

    VOLCANO_INLINE Rect(const PointType &leftTop, const PointType &rightBottom):
        m_left(leftTop.x()),
        m_top(leftTop.y()),
        m_right(rightBottom.x()),
        m_bottom(rightBottom.y())
    {
    }

    VOLCANO_INLINE Rect(const PointType &leftTop, const SizeType &size):
        m_left(leftTop.x()),
        m_top(leftTop.y()),
        m_right(leftTop.x() + size.width()),
        m_bottom(leftTop.y() + size.height())
    {
    }

    VOLCANO_INLINE bool isEmpty(void) const
    {
        return (width() < 1 || height() < 1);
    }

    VOLCANO_INLINE bool isContains(const PointType &r) const
    {
        return (left() <= r.x() && r.x() < width() &&
                top() <= r.y() && r.y() < height());
    }

    VOLCANO_INLINE bool isContains(T x, T y) const
    {
        return contains(PointType(x, y));
    }

    VOLCANO_INLINE PointType center(void) const
    {
        return PointType(left() + width() / 2, top() + height() / 2);
    }

    VOLCANO_INLINE T left(void) const
    {
        return m_left;
    }

    VOLCANO_INLINE void setLeft(T v)
    {
        m_left = v;
    }

    VOLCANO_INLINE T top(void) const
    {
        return m_top;
    }

    VOLCANO_INLINE void setTop(T v)
    {
        m_top = v;
    }

    VOLCANO_INLINE T right(void) const
    {
        return m_right;
    }

    VOLCANO_INLINE void setRight(T v)
    {
        m_right = v;
    }

    VOLCANO_INLINE T bottom(void) const
    {
        return m_bottom;
    }

    VOLCANO_INLINE void setBottom(T v)
    {
        m_bottom = v;
    }

    VOLCANO_INLINE PointType leftTop(void) const
    {
        return point_type(left(), bottom());
    }

    VOLCANO_INLINE PointType leftBottom(void) const
    {
        return PointType(left(), bottom());
    }

    VOLCANO_INLINE PointType rightTop(void) const
    {
        return PointType(right(), top());
    }

    VOLCANO_INLINE PointType rightBottom(void) const
    {
        return PointType(right(), bottom());
    }

    VOLCANO_INLINE T width(void) const
    {
        return m_right - m_left + 1;
    }

    VOLCANO_INLINE void setWidth(T v)
    {
        m_right = m_left + v - 1;
    }

    VOLCANO_INLINE T height(void) const
    {
        return m_bottom - m_top + 1;
    }

    VOLCANO_INLINE void setHeight(T v)
    {
        m_bottom = m_top + v - 1;
    }

    VOLCANO_INLINE SizeType size(void) const
    {
        return SizeType(width(), height());
    }

    VOLCANO_INLINE void set(T left, T top, T right, T bottom)
    {
        m_left = left;
        m_top = top;
        m_right = right;
        m_bottom = bottom;
    }

    VOLCANO_INLINE void set(const PointType &leftTop, const PointType &rightBottom)
    {
        m_left = leftTop.x();
        m_top = leftTop.y();
        m_right = rightBottom.x();
        m_bottom = rightBottom.y();
    }

    VOLCANO_INLINE void normalize(void)
    {
        if (m_left > m_right)
            std::swap(m_left, m_right);
        if (m_top > m_bottom)
            std::swap(m_top, m_bottom);
    }

    Rect normalized(void) const
    {
        Rect tmp(*this);
        tmp.normalize();
        return tmp;
    }

    bool intersects(const Rect &that) const
    {
        if (contains(that.left(), that.top()))
            return true;

        if (contains(that.left(), that.bottom()))
            return true;

        if (contains(that.right(), that.top()))
            return true;

        if (contains(that.right(), that.bottom()))
            return true;

        if (that.contains(left(), top()))
            return true;

        if (that.contains(left(), bottom()))
            return true;

        if (that.contains(right(), top()))
            return true;

        if (that.contains(right(), bottom()))
            return true;

        return false;
    }

    Rect intersected(const Rect &that) const
    {
        // TODO
        VOLCANO_ASSERT(false);
        VOLCANO_UNUSED(that);

        return Rect();
    }

    VOLCANO_INLINE Rect &operator=(const Rect &that)
    {
        m_left = that.m_left;
        m_top = that.m_top;
        m_right = that.m_right;
        m_bottom = that.m_bottom;

        return *this;
    }

private:
    T m_left;
    T m_top;
    T m_right;
    T m_bottom;

    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value);
};

using RectI = Rect<int>;
using RectL = Rect<long>;
using RectF = Rect<float>;
using RectD = Rect<double>;

VOLCANO_END

#endif // VOLCANO_RECT_HPP
