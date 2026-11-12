#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include <google/protobuf/io/zero_copy_stream.h>
#include <gtest/gtest.h>
#include <physfs.h>

#include <Volcano/PhysFile.h>

namespace {

using Volcano::PhysInputStream;
using Volcano::PhysOutputStream;
using Volcano::PhysFile;

static_assert(std::is_base_of_v<google::protobuf::io::ZeroCopyInputStream, PhysInputStream>);
static_assert(std::is_base_of_v<google::protobuf::io::ZeroCopyOutputStream, PhysOutputStream>);
static_assert(!std::is_copy_constructible_v<PhysInputStream>);
static_assert(!std::is_copy_assignable_v<PhysInputStream>);
static_assert(std::is_nothrow_move_constructible_v<PhysInputStream>);

class BasePhysFile: public testing::Test {
protected:
    void SetUp() override {
        root_ = std::filesystem::temp_directory_path() /
            ("volcano-physfile-" + std::to_string(Volcano::Clock::now().time_since_epoch().count()));
        std::filesystem::create_directories(root_ / "first");
        std::filesystem::create_directories(root_ / "second");
        std::ofstream(root_ / "first" / "data.bin", std::ios::binary) << "abcdefghijklmnop";

        const auto first = (root_ / "first").u8string();
        const auto second = (root_ / "second").u8string();
        first_ = std::string(first.begin(), first.end());
        second_ = std::string(second.begin(), second.end());

        ASSERT_NE(PHYSFS_init("BasePhysFileCheck"), 0);
        ASSERT_NE(PHYSFS_mount(first_.c_str(), "first", 1), 0);
        ASSERT_NE(PHYSFS_mount(second_.c_str(), "second", 1), 0);
        ASSERT_NE(PHYSFS_setWriteDir(first_.c_str()), 0);
    }

    void TearDown() override {
        if (PHYSFS_isInit() != 0) {
            EXPECT_NE(PHYSFS_deinit(), 0);
        }

        std::error_code error;
        std::filesystem::remove_all(root_, error);
        EXPECT_FALSE(error);
    }

protected:
    std::filesystem::path root_;
    std::string first_;
    std::string second_;
};

TEST_F(BasePhysFile, ReadsViaZeroCopyStream) {
    PhysInputStream stream(std::filesystem::path("first/data.bin"), 4);
    EXPECT_EQ(stream.fileLength(), 16);

    const void* data = nullptr;
    int size = 0;
    std::string result;
    while (stream.Next(&data, &size)) {
        ASSERT_GT(size, 0);
        result.append(static_cast<const char*>(data), size);
    }
    EXPECT_EQ(result, "abcdefghijklmnop");
    EXPECT_EQ(stream.ByteCount(), 16);
}

TEST_F(BasePhysFile, BackUpAndSkipWorkCorrectly) {
    PhysInputStream stream(std::filesystem::path("first/data.bin"), 8);
    const void* data = nullptr;
    int size = 0;

    ASSERT_TRUE(stream.Next(&data, &size));
    EXPECT_EQ(size, 8);
    EXPECT_EQ(std::string_view(static_cast<const char*>(data), 4), "abcd");

    // Back up 4 bytes
    stream.BackUp(4);
    EXPECT_EQ(stream.ByteCount(), 4);

    ASSERT_TRUE(stream.Next(&data, &size));
    EXPECT_EQ(size, 4);
    EXPECT_EQ(std::string_view(static_cast<const char*>(data), 4), "efgh");
    EXPECT_EQ(stream.ByteCount(), 8);

    // Skip next 4 bytes (ijkl)
    EXPECT_TRUE(stream.Skip(4));
    EXPECT_EQ(stream.ByteCount(), 12);

    ASSERT_TRUE(stream.Next(&data, &size));
    EXPECT_EQ(size, 4);
    EXPECT_EQ(std::string_view(static_cast<const char*>(data), 4), "mnop");
}

TEST_F(BasePhysFile, WritesViaZeroCopyOutputStream) {
    {
        PhysOutputStream output(std::filesystem::path("output.bin"), 4);
        void* data = nullptr;
        int size = 0;

        ASSERT_TRUE(output.Next(&data, &size));
        ASSERT_GE(size, 4);
        std::memcpy(data, "1234", 4);
        output.BackUp(size - 4);

        ASSERT_TRUE(output.Next(&data, &size));
        ASSERT_GE(size, 4);
        std::memcpy(data, "5678", 4);
        output.BackUp(size - 4);

        EXPECT_TRUE(output.Flush());
        EXPECT_EQ(output.ByteCount(), 8);
    }

    {
        PhysInputStream input(std::filesystem::path("first/output.bin"), 16);
        const void* data = nullptr;
        int size = 0;
        ASSERT_TRUE(input.Next(&data, &size));
        EXPECT_EQ(std::string_view(static_cast<const char*>(data), size), "12345678");
    }
}

TEST_F(BasePhysFile, ReportsMissingFile) {
    EXPECT_THROW(PhysInputStream(std::filesystem::path("missing.bin")), std::runtime_error);
}

}
