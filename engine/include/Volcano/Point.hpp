//
//
#ifndef VOLCANO_POINT_HPP
#define VOLCANO_POINT_HPP

#include <type_traits>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

template <typename T>
class Point {
public:
    VOLCANO_INLINE Point(void):
        m_x(0),
        m_y(0)
    {
    }

    VOLCANO_INLINE Point(const Point &that):
        m_x(that.m_x),
        m_y(that.m_y)
    {
    }

    VOLCANO_INLINE Point(T x, T y):
        m_x(x),
        m_y(y)
    {
    }

    VOLCANO_INLINE T x(void) const
    {
        return m_x;
    }

    VOLCANO_INLINE void setX(T v)
    {
        m_x = v;
    }

    VOLCANO_INLINE T y(void) const
    {
        return m_y;
    }

    VOLCANO_INLINE void setY(T v)
    {
        m_y = v;
    }

    VOLCANO_INLINE void set(T x, T y)
    {
        m_x = x;
        m_y = y;
    }

    VOLCANO_INLINE Point &operator=(const Point &that)
    {
        m_x = that.m_x;
        m_y = that.m_y;

        return *this;
    }

private:
    T m_x;
    T m_y;

    // static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value);
};

using PointI = Point<int>;
using PointL = Point<long>;
using PointF = Point<float>;
using PointD = Point<double>;

VOLCANO_END

#endif // VOLCANO_POINT_HPP
