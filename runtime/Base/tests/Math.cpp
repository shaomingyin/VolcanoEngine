#include <limits>

#include <gtest/gtest.h>

#include <Volcano/Math.h>
#include <Volcano/Rect.h>
#include <Volcano/Size.h>

namespace {

template <typename T>
void checkSize() {
    using S = Volcano::BasicSize<T>;
    constexpr S size{6, 8};
    static_assert(size.width == T(6) && size.height == T(8));
    static_assert(size.transposed() == S{8, 6});
    static_assert(!size.empty() && !size.null());
    EXPECT_TRUE((S().empty() && S().null()));
    EXPECT_TRUE(((S{0, 8}.empty() && !S{0, 8}.null())));
    EXPECT_TRUE(((S{6, 0}.empty() && !S{6, 0}.null())));
    EXPECT_TRUE((!S{-6, -8}.empty() && !S{-6, -8}.null()));
    auto copy = size;
    copy.width = 3;
    copy.height = 4;
    EXPECT_TRUE((copy == S{3, 4} && copy != size));
    EXPECT_TRUE((size.width == T(6) && size.height == T(8)));
    EXPECT_TRUE((copy.transposed().transposed() == copy));
}

template <typename T>
void checkRect() {
    using R = Volcano::BasicRect<T>;
    using S = Volcano::BasicSize<T>;
    using V = Eigen::Vector<T, 2>;
    static_assert([] {
        R rect{1, 2, 3, 4};
        rect.setSize({6, 8});
        return rect.size() == S{6, 8} && rect.left == T(1) && rect.top == T(2);
    }());
    const R rect{1, 2, 6, 8};
    EXPECT_TRUE((rect.position() == V(1, 2) && rect.size() == S{6, 8}));
    EXPECT_TRUE((rect.right() == T(7) && rect.bottom() == T(10)));
    EXPECT_TRUE((rect.topLeft() == V(1, 2) && rect.topRight() == V(7, 2)));
    EXPECT_TRUE((rect.bottomLeft() == V(1, 10) && rect.bottomRight() == V(7, 10)));
    EXPECT_TRUE((rect.center() == V(4, 6)));
    EXPECT_TRUE((!rect.empty() && !rect.null()));
    EXPECT_TRUE((R().empty() && R().null()));
    const R line{0, 0, 0, 5};
    EXPECT_TRUE((line.empty() && !line.null() && !line.contains(V(0, 0))));

    R negative{7, 10, -6, -8};
    EXPECT_TRUE((negative.size() == S{-6, -8}));
    EXPECT_TRUE((!negative.empty() && negative.normalized() == rect));
    EXPECT_TRUE((negative.center() == rect.center()));
    EXPECT_TRUE((&negative.normalize() == &negative && negative == rect));
    EXPECT_TRUE((rect.contains(rect.topLeft()) && !rect.contains(rect.bottomRight())));
    EXPECT_TRUE((!rect.contains(T(7), T(2)) && !rect.contains(T(1), T(10))));

    auto moved = rect;
    moved.moveTo(10, 20);
    EXPECT_TRUE((moved.position() == V(10, 20) && moved.size() == rect.size()));
    moved.moveTo(V(0, 2));
    moved.moveCenter(V(10, 10));
    EXPECT_TRUE((moved.center() == V(10, 10) && moved.size() == rect.size()));
    EXPECT_TRUE(((rect.translated(3, -2) == R{4, 0, 6, 8})));
    EXPECT_TRUE((rect.translated(V(3, -2)) == rect.translated(3, -2)));
    auto translated = rect;
    EXPECT_TRUE((&translated.translate(3, -2) == &translated));
    EXPECT_TRUE((translated == rect.translated(3, -2)));
    EXPECT_TRUE((&translated.translate(V(-3, 2)) == &translated && translated == rect));

    auto resized = rect;
    resized.setLeft(-1);
    resized.setTop(0);
    EXPECT_TRUE((resized.right() == rect.right() && resized.bottom() == rect.bottom()));
    resized.setRight(9);
    resized.setBottom(12);
    EXPECT_TRUE((resized.size() == S{10, 12}));
    resized.setSize(S{4, 6});
    EXPECT_TRUE((resized.position() == V(-1, 0) && resized.size() == S{4, 6}));
    auto size = resized.size();
    size.width = -4;
    size.height = 0;
    EXPECT_TRUE((resized.size() == S{4, 6}));
    resized.setSize(size);
    EXPECT_TRUE((resized.left == T(-1) && resized.top == T(0) && resized.size() == size));
    EXPECT_TRUE((resized.empty() && resized.size().empty() && !resized.size().null()));
    EXPECT_TRUE(((rect.adjusted(-1, -2, 3, 4) == R{0, 0, 10, 14})));
    auto adjusted = rect;
    EXPECT_TRUE((&adjusted.adjust(1, 1, -1, -1) == &adjusted));
    EXPECT_TRUE(((adjusted == R{2, 3, 4, 6})));
    EXPECT_TRUE(((rect.transposed() == R{1, 2, 8, 6})));

    const R other{5, 8, 6, 8};
    EXPECT_TRUE(((rect.intersected(other) == R{5, 8, 2, 2})));
    EXPECT_TRUE(((rect.united(other) == R{1, 2, 10, 14})));
    EXPECT_TRUE((rect.contains(adjusted) && rect.contains(rect)));
    EXPECT_TRUE((!rect.contains(other) && !rect.contains(R()) && !R().contains(rect)));
    const R touching{7, 2, 3, 8};
    EXPECT_TRUE((!rect.intersects(touching) && rect.intersected(touching).null()));
    EXPECT_TRUE((!rect.intersects(line) && rect.intersected(line).null()));
    EXPECT_TRUE((rect.united(line) == rect && line.united(rect) == rect));
    EXPECT_TRUE((rect != other));

    const R cases[] = {{}, {0, 0, 3, 2}, {3, 2, -3, -2}, {2, 1, 3, 3}, {3, 0, 2, 2}, {1, 1, 0, 2}};
    for (const auto& a : cases) {
        for (const auto& b : cases) {
            const auto intersection = a.intersected(b);
            const auto united = a.united(b);
            EXPECT_TRUE((intersection == b.intersected(a)));
            EXPECT_TRUE((united == b.united(a)));
            EXPECT_TRUE((a.intersects(b) == !intersection.empty()));
            for (int y = -1; y <= 5; ++y) {
                for (int x = -1; x <= 5; ++x) {
                    EXPECT_TRUE((intersection.contains(T(x), T(y)) == (a.contains(T(x), T(y)) && b.contains(T(x), T(y)))));
                    if (a.contains(T(x), T(y)) || b.contains(T(x), T(y))) {
                        EXPECT_TRUE((united.contains(T(x), T(y))));
                    }
                }
            }
        }
    }
}

} // namespace

TEST(BaseMath, SizeInt) {
    checkSize<int>();
}

TEST(BaseMath, SizeFloat) {
    checkSize<float>();
}

TEST(BaseMath, SizeDouble) {
    checkSize<double>();
}

TEST(BaseMath, RectInt) {
    checkRect<int>();
}

TEST(BaseMath, RectFloat) {
    checkRect<float>();
}

TEST(BaseMath, RectDouble) {
    checkRect<double>();
}

TEST(BaseMath, Angles) {
    EXPECT_FLOAT_EQ(Volcano::Eigen::radians(180.0f), std::numbers::pi_v<float>);
    EXPECT_DOUBLE_EQ(Volcano::Eigen::degrees(std::numbers::pi), 180.0);
}

TEST(BaseMath, NormalMatrixRejectsSingularAndNonfiniteTransforms) {
    Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    transform.scale(Eigen::Vector3f{2, 4, 8});
    EXPECT_TRUE(Volcano::Eigen::normalMatrix(transform).isApprox(
        Eigen::Vector3f{0.5f, 0.25f, 0.125f}.asDiagonal().toDenseMatrix()));
    transform.linear()(0, 0) = 0;
    EXPECT_THROW(Volcano::Eigen::normalMatrix(transform), std::invalid_argument);
    transform = Eigen::Affine3f::Identity();
    transform.translation().x() = std::numeric_limits<float>::infinity();
    EXPECT_THROW(Volcano::Eigen::normalMatrix(transform), std::invalid_argument);
}
