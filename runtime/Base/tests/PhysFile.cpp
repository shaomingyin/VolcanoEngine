#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>
#include <physfs.h>

#include <Volcano/Buffer.h>
#include <Volcano/PhysFile.h>
#include <Volcano/Stream.h>

namespace {

using Volcano::PhysFile;

static_assert(std::is_base_of_v<Volcano::Stream, PhysFile>);
static_assert(!std::is_copy_constructible_v<PhysFile>);
static_assert(!std::is_copy_assignable_v<PhysFile>);
static_assert(std::is_nothrow_move_constructible_v<PhysFile>);
static_assert(std::is_nothrow_destructible_v<PhysFile>);

class BasePhysFile: public testing::Test {
protected:
    void SetUp() override {
        root_ = std::filesystem::temp_directory_path() /
            ("volcano-physfile-" + std::to_string(Volcano::Clock::now().time_since_epoch().count()));
        std::filesystem::create_directories(root_ / "first");
        std::filesystem::create_directories(root_ / "second");
        std::ofstream(root_ / "first" / "data.bin", std::ios::binary) << "abc";
        std::ofstream(root_ / "second" / "data.bin", std::ios::binary) << "xyz";

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

TEST_F(BasePhysFile, DirectReadsAndAutomaticClose) {
    {
        PhysFile file(PHYSFS_openRead("first/data.bin"), PhysFile::OpenMode::ReadOnly);
        ASSERT_NE(file.get(), nullptr);
        std::array<char, 3> bytes{};
        ASSERT_EQ(PHYSFS_readBytes(file.get(), bytes.data(), bytes.size()), 3);
        EXPECT_EQ(std::string(bytes.begin(), bytes.end()), "abc");
        EXPECT_EQ(PHYSFS_unmount(first_.c_str()), 0);
    }

    EXPECT_NE(PHYSFS_unmount(first_.c_str()), 0);
}

TEST_F(BasePhysFile, PathConstructorOpensForReading) {
    {
        PhysFile file(std::filesystem::path("first/data.bin"));
        std::array<char, 3> bytes{};
        ASSERT_EQ(PHYSFS_readBytes(file.get(), bytes.data(), bytes.size()), 3);
        EXPECT_EQ(std::string(bytes.begin(), bytes.end()), "abc");
    }

    EXPECT_NE(PHYSFS_unmount(first_.c_str()), 0);
}

TEST_F(BasePhysFile, PathConstructorReportsMissingFile) {
    try {
        PhysFile file(std::filesystem::path("missing.bin"));
        FAIL() << "Missing file must throw";
    } catch (const std::runtime_error& error) {
        EXPECT_NE(std::string(error.what()).find("missing.bin"), std::string::npos);
    }
}

TEST_F(BasePhysFile, ExceptionUnwindingClosesHandle) {
    EXPECT_THROW({
        PhysFile file(PHYSFS_openRead("first/data.bin"), PhysFile::OpenMode::ReadOnly);
        ASSERT_NE(file.get(), nullptr);
        throw std::runtime_error("Operation failed");
    }, std::runtime_error);

    EXPECT_NE(PHYSFS_unmount(first_.c_str()), 0);
}

TEST_F(BasePhysFile, MoveConstructionTransfersOwnership) {
    {
        PhysFile source(PHYSFS_openRead("first/data.bin"), PhysFile::OpenMode::ReadOnly);
        ASSERT_NE(source.get(), nullptr);
        const auto handle = source.get();
        PhysFile destination(std::move(source));
        EXPECT_EQ(source.get(), nullptr);
        EXPECT_EQ(destination.get(), handle);
        EXPECT_EQ(PHYSFS_unmount(first_.c_str()), 0);
    }

    EXPECT_NE(PHYSFS_unmount(first_.c_str()), 0);
}

TEST_F(BasePhysFile, MoveAssignmentClosesDestinationBeforeTakingOwnership) {
    PhysFile destination(PHYSFS_openRead("first/data.bin"), PhysFile::OpenMode::ReadOnly);
    PhysFile source(PHYSFS_openRead("second/data.bin"), PhysFile::OpenMode::ReadOnly);
    ASSERT_NE(destination.get(), nullptr);
    ASSERT_NE(source.get(), nullptr);
    const auto handle = source.get();

    destination = std::move(source);
    EXPECT_EQ(source.get(), nullptr);
    EXPECT_EQ(destination.get(), handle);
    EXPECT_NE(PHYSFS_unmount(first_.c_str()), 0);
    EXPECT_EQ(PHYSFS_unmount(second_.c_str()), 0);

    auto& same = destination;
    destination = std::move(same);
    EXPECT_EQ(destination.get(), handle);
    destination.close();
    EXPECT_NE(PHYSFS_unmount(second_.c_str()), 0);
}

TEST_F(BasePhysFile, ExplicitCloseFlushesBufferedWritesAndIsRepeatable) {
    PhysFile file(PHYSFS_openWrite("output.bin"), PhysFile::OpenMode::WriteOnly);
    ASSERT_NE(file.get(), nullptr);
    ASSERT_NE(PHYSFS_setBuffer(file.get(), 1024), 0);
    ASSERT_EQ(PHYSFS_writeBytes(file.get(), "saved", 5), 5);
    file.close();
    EXPECT_EQ(file.get(), nullptr);
    EXPECT_NO_THROW(file.close());

    PhysFile input(PHYSFS_openRead("first/output.bin"), PhysFile::OpenMode::ReadOnly);
    ASSERT_NE(input.get(), nullptr);
    std::array<char, 5> bytes{};
    ASSERT_EQ(PHYSFS_readBytes(input.get(), bytes.data(), bytes.size()), 5);
    EXPECT_EQ(std::string(bytes.begin(), bytes.end()), "saved");
}

TEST_F(BasePhysFile, EmptyAndFailedOpenHandlesAreSafe) {
    PhysFile empty;
    EXPECT_EQ(empty.get(), nullptr);
    EXPECT_NO_THROW(empty.close());

    PhysFile missing(PHYSFS_openRead("missing.bin"), PhysFile::OpenMode::ReadOnly);
    EXPECT_EQ(missing.get(), nullptr);
    EXPECT_EQ(PHYSFS_getLastErrorCode(), PHYSFS_ERR_NOT_FOUND);
    EXPECT_NO_THROW(missing.close());

    PhysFile destination(std::move(empty));
    EXPECT_EQ(destination.get(), nullptr);
}

TEST_F(BasePhysFile, ReadsFromCurrentPositionAndSupportsSeeking) {
    PhysFile file("first/data.bin");
    EXPECT_TRUE(file.isOpen());
    EXPECT_EQ(file.size(), 3);
    EXPECT_EQ(file.tell(), 0);
    EXPECT_FALSE(file.eof());

    std::array<std::uint8_t, 1> first{};
    EXPECT_EQ(file.read(first), 1);
    EXPECT_EQ(first[0], 'a');
    EXPECT_EQ(file.tell(), 1);
    EXPECT_EQ(file.readAll(), Volcano::ByteArray({'b', 'c'}));
    EXPECT_TRUE(file.eof());
    EXPECT_EQ(file.read(first), 0);
    EXPECT_TRUE(file.readAll().empty());

    file.seek(0);
    EXPECT_FALSE(file.eof());
    EXPECT_EQ(file.readAll(), Volcano::ByteArray({'a', 'b', 'c'}));
    file.seek(file.size());
    EXPECT_TRUE(file.eof());
    file.seek(4);
    EXPECT_EQ(file.tell(), 4);
    EXPECT_EQ(file.read(first), 0);
}

TEST_F(BasePhysFile, ExactReadsRejectEarlyEof) {
    PhysFile file("first/data.bin");
    std::array<std::uint8_t, 3> bytes{};
    file.readExact(bytes);
    EXPECT_EQ(bytes, (std::array<std::uint8_t, 3>{'a', 'b', 'c'}));
    EXPECT_NO_THROW(file.readExact({}));

    file.seek(0);
    std::array<std::uint8_t, 4> oversized{};
    EXPECT_THROW(file.readExact(oversized), std::runtime_error);
    EXPECT_EQ(file.tell(), 3);
    EXPECT_EQ(oversized[0], 'a');
}

TEST_F(BasePhysFile, ReadAllCrossesBufferBoundaries) {
    const std::string contents(10000, 'z');
    std::ofstream(root_ / "first" / "large.bin", std::ios::binary) << contents;
    PhysFile file("first/large.bin");
    file.seek(10);
    EXPECT_EQ(file.readAll(), Volcano::ByteArray(9990, 'z'));
}

TEST_F(BasePhysFile, PointerReadsValidateBuffersAndReturnShortCounts) {
    PhysFile file("first/data.bin");
    std::array<char, 5> bytes{};
    EXPECT_THROW(file.read(nullptr, 1), std::invalid_argument);
    EXPECT_EQ(file.read(nullptr, 0), 0);
    EXPECT_EQ(file.read(bytes.data(), bytes.size()), 3);
    EXPECT_EQ(std::string(bytes.data(), 3), "abc");
}

TEST_F(BasePhysFile, ClosedAndMovedFromFilesRejectOperations) {
    PhysFile closed;
    std::array<std::uint8_t, 1> bytes{};
    EXPECT_FALSE(closed.isOpen());
    EXPECT_FALSE(closed.isReadable());
    EXPECT_FALSE(closed.isWritable());
    EXPECT_EQ(closed.read(nullptr, 0), 0);
    EXPECT_EQ(closed.write(nullptr, 0), 0);
    EXPECT_NO_THROW(closed.writeAll({}));
    EXPECT_THROW(closed.write(bytes), std::logic_error);
    EXPECT_THROW(closed.writeAll(bytes), std::logic_error);
    EXPECT_THROW(closed.flush(), std::logic_error);
    EXPECT_THROW(closed.read(bytes), std::logic_error);
    EXPECT_NO_THROW(closed.readExact({}));
    EXPECT_THROW(closed.readExact(bytes), std::logic_error);
    EXPECT_THROW(closed.readAll(), std::logic_error);
    EXPECT_THROW(closed.seek(0), std::logic_error);
    EXPECT_THROW(closed.tell(), std::logic_error);
    EXPECT_THROW(closed.size(), std::logic_error);
    EXPECT_THROW(closed.eof(), std::logic_error);

    PhysFile source("first/data.bin");
    PhysFile destination(std::move(source));
    EXPECT_FALSE(source.isOpen());
    EXPECT_THROW(source.read(bytes), std::logic_error);
    destination.close();
    EXPECT_THROW(destination.readAll(), std::logic_error);
}

TEST_F(BasePhysFile, ReadingWriteOnlyHandleReportsError) {
    PhysFile file(PHYSFS_openWrite("write-only.bin"), PhysFile::OpenMode::WriteOnly);
    ASSERT_TRUE(file.isOpen());
    std::array<std::uint8_t, 1> bytes{};
    EXPECT_THROW(file.read(bytes), std::runtime_error);
}

TEST_F(BasePhysFile, PathConstructorSupportsUtf8AndRejectsEmbeddedNul) {
    std::ofstream(root_ / "first" / std::filesystem::path(u8"资源.bin"), std::ios::binary) << "ok";
    PhysFile file(std::filesystem::path(u8"first/资源.bin"));
    EXPECT_EQ(file.readAll(), Volcano::ByteArray({'o', 'k'}));

    std::string path = "first/data.bin";
    path.push_back('\0');
    path += "ignored";
    EXPECT_THROW(PhysFile{std::filesystem::path(path)}, std::invalid_argument);
    EXPECT_THROW(PhysFile{"first/../second/data.bin"}, std::runtime_error);
}


TEST_F(BasePhysFile, FileAndBufferUseTheSameStreamReadInterface) {
    const auto readPayload = [](Volcano::Stream& stream) {
        stream.seek(1);
        Volcano::ByteArray bytes(2);
        stream.readExact(bytes);
        return bytes;
    };

    Volcano::Buffer buffer({'a', 'b', 'c'});
    buffer.open(Volcano::Buffer::ReadOnly);
    PhysFile file("first/data.bin");
    EXPECT_TRUE(file.isReadable());
    EXPECT_FALSE(file.isWritable());
    EXPECT_FALSE(file.isSequential());
    EXPECT_EQ(readPayload(file), readPayload(buffer));
    EXPECT_EQ(file.tell(), buffer.tell());
    EXPECT_EQ(file.size(), buffer.size());
    EXPECT_NO_THROW(file.flush());
    EXPECT_THROW(file.write(Volcano::ByteArray{1}), std::runtime_error);
}

TEST_F(BasePhysFile, FileAndBufferUseTheSameStreamWriteInterface) {
    const auto writePayload = [](Volcano::Stream& stream) {
        stream.writeAll(Volcano::ByteArray{1, 0, 2});
        EXPECT_EQ(stream.write("xyz", 3), 3);
        stream.flush();
    };

    Volcano::Buffer buffer;
    buffer.open(Volcano::Buffer::ReadWrite);
    PhysFile file("stream.bin", PhysFile::OpenMode::WriteOnly);
    EXPECT_FALSE(file.isReadable());
    EXPECT_TRUE(file.isWritable());
    EXPECT_THROW(file.write(nullptr, 1), std::invalid_argument);
    ASSERT_NE(PHYSFS_setBuffer(file.get(), 1024), 0);
    writePayload(file);
    writePayload(buffer);
    EXPECT_EQ(file.tell(), buffer.tell());
    EXPECT_EQ(file.size(), buffer.size());
    file.close();
    EXPECT_FALSE(file.isWritable());

    PhysFile input("first/stream.bin");
    EXPECT_EQ(input.readAll(), buffer.data());
}

TEST_F(BasePhysFile, WriteModeTruncatesAndAppendModePreservesContents) {
    {
        PhysFile file("data.bin", PhysFile::OpenMode::WriteOnly);
        EXPECT_EQ(file.size(), 0);
        file.writeAll(Volcano::ByteArray{'d', 'e'});
        file.close();
    }
    {
        PhysFile file("data.bin", PhysFile::OpenMode::Append);
        EXPECT_EQ(file.tell(), 2);
        EXPECT_FALSE(file.isReadable());
        EXPECT_TRUE(file.isWritable());
        file.writeAll(Volcano::ByteArray{'f'});
        file.close();
    }

    PhysFile input("first/data.bin");
    EXPECT_EQ(input.readAll(), Volcano::ByteArray({'d', 'e', 'f'}));
}

TEST_F(BasePhysFile, MovesPreserveWriteCapability) {
    PhysFile original("move.bin", PhysFile::OpenMode::WriteOnly);
    PhysFile moved(std::move(original));
    EXPECT_FALSE(original.isWritable());
    EXPECT_TRUE(moved.isWritable());
    moved.writeAll(Volcano::ByteArray{1});

    PhysFile destination("first/data.bin");
    destination = std::move(moved);
    EXPECT_FALSE(moved.isWritable());
    EXPECT_TRUE(destination.isWritable());
    EXPECT_FALSE(destination.isReadable());
    destination.writeAll(Volcano::ByteArray{2});
    destination.close();

    PhysFile input("first/move.bin");
    EXPECT_EQ(input.readAll(), Volcano::ByteArray({1, 2}));
}

TEST_F(BasePhysFile, RejectsInvalidOpenModeBeforeOpeningFile) {
    EXPECT_THROW((PhysFile{"data.bin", static_cast<PhysFile::OpenMode>(99)}), std::invalid_argument);
    PhysFile file("first/data.bin");
    EXPECT_EQ(file.readAll(), Volcano::ByteArray({'a', 'b', 'c'}));
}

} // namespace
