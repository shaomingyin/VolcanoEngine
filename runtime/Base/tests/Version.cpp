#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include <Volcano/Version.h>

using Volcano::VersionNumber;

static_assert(VersionNumber().major() == 0);
static_assert(VersionNumber().minor() == 0);
static_assert(VersionNumber().patch() == 0);
static_assert(VersionNumber().extraInfo().empty());
static_assert(VersionNumber(1, 2, 3).major() == 1);
static_assert(VersionNumber(1, 2, 3).minor() == 2);
static_assert(VersionNumber(1, 2, 3).patch() == 3);
static_assert(VersionNumber(1, 9, 9) < VersionNumber(2, 0, 0));
static_assert(VersionNumber(1, 2, 9) < VersionNumber(1, 3, 0));
static_assert(VersionNumber(1, 2, 3) < VersionNumber(1, 2, 4));
static_assert(VersionNumber(1, 2, 3, "beta.1") == VersionNumber(1, 2, 3));
static_assert(!std::is_nothrow_copy_assignable_v<VersionNumber>);

TEST(BaseVersion, Checks) {
    EXPECT_TRUE((VersionNumber().toString() == "0.0.0"));
    EXPECT_TRUE((VersionNumber(1, 2, 3).toString() == "1.2.3"));
    const VersionNumber beta(1, 2, 3, "beta.1");
    EXPECT_TRUE((beta.extraInfo() == "beta.1"));
    EXPECT_TRUE((beta.toString() == "1.2.3-beta.1"));
    const VersionNumber release(1, 2, 3);
    EXPECT_TRUE((beta == release && !(beta != release)));
    EXPECT_TRUE((beta <= release && beta >= release));
    EXPECT_TRUE((!(beta < release) && !(beta > release)));
    EXPECT_TRUE((VersionNumber(2, 0, 0) > beta));
    EXPECT_TRUE((VersionNumber(1, 2, 2) != beta));

    const std::string longInfo(200, 'x');
    VersionNumber copy;
    {
        const VersionNumber original(4, 5, 6, longInfo);
        copy = original;
    }

    EXPECT_TRUE((copy.extraInfo() == longInfo));
    EXPECT_TRUE((copy.toString() == "4.5.6-" + longInfo));
    const VersionNumber copied(copy);
    const VersionNumber moved(std::move(copy));
    EXPECT_TRUE((copied.extraInfo() == longInfo && moved.extraInfo() == longInfo));
    EXPECT_TRUE((Volcano::Version == VersionNumber(VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH)));
    EXPECT_TRUE((Volcano::Version.toString() == VOLCANO_VERSION_STR));
}
