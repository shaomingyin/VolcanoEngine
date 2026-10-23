#include <gtest/gtest.h>
#include <limits>

#include <Volcano/Math/Math.h>

namespace {

template <typename Operation>
void invalid(Operation operation) {
    EXPECT_THROW(operation(), std::invalid_argument);
}

template <typename T, std::size_t N>
void checkBox() {
    using Box = Volcano::Math::BoundingBox<T, N>;
    using V = Volcano::Math::Vector<T, N>;
    Box empty;
    EXPECT_TRUE((empty.empty() && empty.center() == V() && empty.extent() == V()));
    EXPECT_TRUE((!empty.contains(V()) && !empty.contains(empty) && !empty.intersects(empty)));
    EXPECT_TRUE((std::isinf(empty.distance(V()))));

    Box box(V(T(-1)), V(T(1)));
    EXPECT_TRUE((box.center() == V() && box.extent() == V(T(1))));
    EXPECT_TRUE((box.contains(V(T(-1))) && box.contains(V(T(1)))));
    EXPECT_TRUE((box.contains(box)));
    EXPECT_TRUE((box.distance(V()) == T{}));
    V outside;
    outside[0] = T(4); outside[1] = T(5);
    EXPECT_TRUE((box.distance(outside) == T(5)));

    const Box touching(V(T(1)), V(T(2)));
    const Box separated(V(T(2)), V(T(3)));
    EXPECT_TRUE((box.intersects(touching) && touching.intersects(box)));
    EXPECT_TRUE((!box.intersects(separated) && !separated.intersects(box)));
    EXPECT_TRUE((!box.contains(touching)));
    EXPECT_TRUE((!box.intersects(empty) && !empty.intersects(box)));
    EXPECT_TRUE((!box.contains(empty) && !empty.contains(box)));
    const Box point(V(T(1)), V(T(1)));
    EXPECT_TRUE((!point.empty() && point.extent() == V()));
    EXPECT_TRUE((box.contains(point) && box.intersects(point)));
    box.extend(separated);
    EXPECT_TRUE((box.minimum() == V(T(-1)) && box.maximum() == V(T(3))));
    box.extend(empty);
    box.extend(box);
    EXPECT_TRUE((box.minimum() == V(T(-1)) && box.maximum() == V(T(3))));
    empty.extend(box);
    EXPECT_TRUE((empty.minimum() == box.minimum() && empty.maximum() == box.maximum()));
    empty.reset();
    empty.extend(V(T(2)));
    EXPECT_TRUE((empty.minimum() == V(T(2)) && empty.maximum() == V(T(2))));

    Box inverted(V(T(2)), V(T(1)));
    EXPECT_TRUE((inverted.empty()));
    inverted.extend(V(T(4)));
    EXPECT_TRUE((inverted.minimum() == V(T(4)) && inverted.maximum() == V(T(4))));

    const T maximum = std::numeric_limits<T>::max();
    const Box large(V(maximum / T(2)), V(maximum));
    EXPECT_TRUE((std::isfinite(large.center()[0]) && large.center()[0] > maximum / T(2)));
    const Box symmetric(V(-maximum), V(maximum));
    EXPECT_TRUE((symmetric.center() == V() && symmetric.extent() == V(maximum)));

    V bad;
    bad[N - 1] = std::numeric_limits<T>::quiet_NaN();
    invalid([&] { Box invalidBox(bad, V()); });
    invalid([&] { box.extend(bad); });
    invalid([&] { box.distance(bad); });
    EXPECT_TRUE((!box.contains(bad)));
    EXPECT_TRUE((box.minimum() == V(T(-1)) && box.maximum() == V(T(3))));
    bad[N - 1] = std::numeric_limits<T>::infinity();
    invalid([&] { Box invalidBox(V(), bad); });
    invalid([&] { box.extend(bad); });

    Volcano::Math::Matrix<T, N + 1> matrix;
    matrix[0][0] = T(-2); matrix[1][0] = T(3);
    for (std::size_t i = 0; i < N; ++i) {
        matrix[N][i] = T(4);
    }
    const Volcano::Math::Affine<T, N> affine(matrix);
    const Box unit(V(T(-1)), V(T(1)));
    const auto transformed = affine.transform(unit);
    V minimum(T(3)), upper(T(5));
    minimum[0] = T(-1); upper[0] = T(9);
    EXPECT_TRUE((transformed.minimum() == minimum && transformed.maximum() == upper));
    EXPECT_TRUE((affine.transform(Box()).empty()));
    for (std::size_t corner = 0; corner < (std::size_t(1) << N); ++corner) {
        V point;
        for (std::size_t i = 0; i < N; ++i) {
            point[i] = (corner & (std::size_t(1) << i)) ? T(1) : T(-1);
        }
        EXPECT_TRUE((transformed.contains(affine.transform(point))));
    }
}

template <typename T>
void checkRect() {
    using R = Volcano::Math::BasicRect<T>;
    using V = Volcano::Math::Vector2<T>;
    const R rect{1, 2, 6, 8};
    EXPECT_TRUE((rect.position() == V(1, 2) && rect.size() == V(6, 8)));
    EXPECT_TRUE((rect.right() == T(7) && rect.bottom() == T(10)));
    EXPECT_TRUE((rect.topLeft() == V(1, 2) && rect.topRight() == V(7, 2)));
    EXPECT_TRUE((rect.bottomLeft() == V(1, 10) && rect.bottomRight() == V(7, 10)));
    EXPECT_TRUE((rect.center() == V(4, 6)));
    EXPECT_TRUE((!rect.empty() && !rect.null()));
    EXPECT_TRUE((R().empty() && R().null()));
    const R line{0, 0, 0, 5};
    EXPECT_TRUE((line.empty() && !line.null() && !line.contains(V(0, 0))));

    R negative{7, 10, -6, -8};
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
    EXPECT_TRUE((resized.size() == V(10, 12)));
    resized.setSize(V(4, 6));
    EXPECT_TRUE((resized.position() == V(-1, 0) && resized.size() == V(4, 6)));
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

static_assert(std::is_same_v<Volcano::Math::Rect, Volcano::Math::BasicRect<int>>);
static_assert(std::is_same_v<Volcano::Math::Rectf, Volcano::Math::BasicRect<float>>);
static_assert(Volcano::Math::BoundingBox3f().empty());
static_assert(Volcano::Math::Rect{5, 5, -5, -5}.center() == Volcano::Math::Vector2i(2, 2));
static_assert(Volcano::Math::Rectf{0, 0, 3, 3}.contains(Volcano::Math::Vector2f(1, 1)));

TEST(MathGeometry, RectInt) {
    checkRect<int>();
}

TEST(MathGeometry, RectFloat) {
    checkRect<float>();
}

TEST(MathGeometry, RectDouble) {
    checkRect<double>();
}

TEST(MathGeometry, BoxFloat2) {
    checkBox<float, 2>();
}

TEST(MathGeometry, BoxFloat3) {
    checkBox<float, 3>();
}

TEST(MathGeometry, BoxDouble2) {
    checkBox<double, 2>();
}

TEST(MathGeometry, BoxDouble3) {
    checkBox<double, 3>();
}
