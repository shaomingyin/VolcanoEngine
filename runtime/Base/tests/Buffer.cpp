#include <array>
#include <limits>
#include <utility>

#include <gtest/gtest.h>

#include <Volcano/Buffer.h>

namespace {

using Volcano::Buffer;
using Volcano::ByteArray;

TEST(BaseBuffer, OwnsDataAndStartsClosed) {
    Buffer empty;
    EXPECT_TRUE(empty.data().empty());
    ByteArray original{1, 0, 2};
    Buffer buffer(original);
    original[0] = 9;
    EXPECT_EQ(buffer.data(), ByteArray({1, 0, 2}));
    EXPECT_FALSE(buffer.isOpen());
    EXPECT_FALSE(buffer.isReadable());
    EXPECT_FALSE(buffer.isWritable());
    EXPECT_FALSE(buffer.isSequential());
    EXPECT_EQ(buffer.tell(), 0);
    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(&buffer.buffer(), &buffer.data());
    EXPECT_EQ(&std::as_const(buffer).buffer(), &buffer.data());

    std::array<std::uint8_t, 1> byte{};
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(buffer.read(byte), "[Aa]ssert");
    EXPECT_DEATH(buffer.write(byte), "[Aa]ssert");
    EXPECT_DEATH(buffer.seek(0), "[Aa]ssert");
#endif
    EXPECT_EQ(buffer.read(nullptr, 0), 0);
    EXPECT_EQ(buffer.write(nullptr, 0), 0);
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(buffer.read(nullptr, 1), "[Aa]ssert");
    EXPECT_DEATH(buffer.write(nullptr, 1), "[Aa]ssert");
#endif
    EXPECT_NO_THROW(buffer.flush());
}

TEST(BaseBuffer, ReadsShortTransfersAndCurrentPositionToEof) {
    Buffer buffer({1, 0, 2, 3});
    buffer.open(Buffer::ReadOnly);
    EXPECT_TRUE(buffer.isReadable());
    EXPECT_FALSE(buffer.isWritable());
    std::array<std::uint8_t, 3> bytes{};
    EXPECT_EQ(buffer.read(bytes.data(), bytes.size()), 3);
    EXPECT_EQ(bytes, (std::array<std::uint8_t, 3>{1, 0, 2}));
    EXPECT_EQ(buffer.tell(), 3);
    EXPECT_EQ(buffer.read(bytes), 1);
    EXPECT_EQ(bytes[0], 3);
    EXPECT_EQ(buffer.read(bytes), 0);
    EXPECT_EQ(buffer.tell(), buffer.size());
    EXPECT_TRUE(buffer.readAll().empty());
    EXPECT_THROW(buffer.readExact(bytes), std::runtime_error);
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(buffer.write(bytes), "[Aa]ssert");
#endif

    buffer.seek(1);
    Volcano::Stream &stream = buffer;
    EXPECT_EQ(stream.readAll(), ByteArray({0, 2, 3}));
    buffer.seek(0);
    stream.readExact(bytes);
    EXPECT_EQ(bytes, (std::array<std::uint8_t, 3>{1, 0, 2}));
}

TEST(BaseBuffer, WriteOnlyPreservesDataWithoutTruncate) {
    Buffer buffer({1, 2, 3});
    buffer.open(Buffer::WriteOnly);
    EXPECT_FALSE(buffer.isReadable());
    EXPECT_TRUE(buffer.isWritable());
    EXPECT_EQ(buffer.write(ByteArray{9}), 1);
    EXPECT_EQ(buffer.data(), ByteArray({9, 2, 3}));
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(buffer.readAll(), "[Aa]ssert");
#endif
    buffer.close();
    EXPECT_EQ(buffer.tell(), 0);
    EXPECT_FALSE(buffer.isOpen());
    EXPECT_EQ(buffer.data(), ByteArray({9, 2, 3}));

    buffer.open(Buffer::ReadWrite);
    EXPECT_TRUE(buffer.isReadable());
    EXPECT_TRUE(buffer.isWritable());
    std::array<std::uint8_t, 1> first{};
    buffer.readExact(first);
    EXPECT_EQ(first[0], 9);
    buffer.writeAll(ByteArray{4, 0, 5});
    EXPECT_EQ(buffer.tell(), 4);
    EXPECT_EQ(buffer.size(), 4);
    buffer.seek(0);
    EXPECT_EQ(buffer.readAll(), ByteArray({9, 4, 0, 5}));
}

TEST(BaseBuffer, AppendAlwaysWritesAtTheCurrentEnd) {
    Buffer buffer({1, 2});
    buffer.open(Buffer::ReadOnly | Buffer::Append);
    EXPECT_TRUE(buffer.isReadable());
    EXPECT_TRUE(buffer.isWritable());
    EXPECT_EQ(buffer.tell(), 2);
    buffer.seek(0);
    EXPECT_EQ(buffer.write(ByteArray{3}), 1);
    EXPECT_EQ(buffer.data(), ByteArray({1, 2, 3}));
    EXPECT_EQ(buffer.tell(), 3);
    buffer.buffer().push_back(4);
    buffer.seek(1);
    buffer.writeAll(ByteArray{5});
    EXPECT_EQ(buffer.data(), ByteArray({1, 2, 3, 4, 5}));
    EXPECT_EQ(buffer.tell(), 5);
    buffer.close();
    buffer.open(Buffer::Append);
    EXPECT_FALSE(buffer.isReadable());
    EXPECT_TRUE(buffer.isWritable());
    EXPECT_EQ(buffer.tell(), 5);
}

TEST(BaseBuffer, TruncateClearsOnlyOnOpenAndCanCombineWithAppend) {
    Buffer buffer({1, 2, 3});
    buffer.open(Buffer::ReadWrite | Buffer::Truncate | Buffer::Append);
    EXPECT_TRUE(buffer.isReadable());
    EXPECT_TRUE(buffer.isWritable());
    EXPECT_TRUE(buffer.data().empty());
    EXPECT_EQ(buffer.tell(), 0);
    buffer.writeAll(ByteArray{7, 8});
    EXPECT_EQ(buffer.data(), ByteArray({7, 8}));
    buffer.close();
    buffer.open(Buffer::Truncate);
    EXPECT_TRUE(buffer.isWritable());
    EXPECT_FALSE(buffer.isReadable());
    EXPECT_TRUE(buffer.data().empty());
}

TEST(BaseBuffer, WritableSeekZeroFillsAndFailuresPreserveState) {
    Buffer buffer({1, 2});
    buffer.open(Buffer::ReadOnly);
    buffer.seek(2);
    EXPECT_THROW(buffer.seek(3), std::out_of_range);
    EXPECT_EQ(buffer.tell(), 2);
    EXPECT_EQ(buffer.data(), ByteArray({1, 2}));
    buffer.close();
    buffer.open(Buffer::ReadWrite);
    buffer.seek(5);
    EXPECT_EQ(buffer.data(), ByteArray({1, 2, 0, 0, 0}));
    EXPECT_TRUE(buffer.readAll().empty());
    buffer.writeAll(ByteArray{9});
    EXPECT_EQ(buffer.data(), ByteArray({1, 2, 0, 0, 0, 9}));
    EXPECT_EQ(buffer.tell(), 6);
    EXPECT_THROW(buffer.seek(std::numeric_limits<std::size_t>::max()), std::length_error);
    EXPECT_EQ(buffer.tell(), 6);
    EXPECT_EQ(buffer.data(), ByteArray({1, 2, 0, 0, 0, 9}));
}

TEST(BaseBuffer, RejectsInvalidModesAndReplacingOpenData) {
    Buffer buffer({1, 2});
    EXPECT_THROW(buffer.open(Buffer::NotOpen), std::invalid_argument);
    EXPECT_THROW(buffer.open(static_cast<Buffer::OpenMode>(128)), std::invalid_argument);
    EXPECT_THROW(buffer.open(Buffer::ReadOnly | static_cast<Buffer::OpenMode>(128)), std::invalid_argument);
    EXPECT_FALSE(buffer.isOpen());
    EXPECT_EQ(buffer.data(), ByteArray({1, 2}));
    buffer.setData({3, 4, 5});
    buffer.open(Buffer::ReadWrite);
    buffer.seek(1);
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(buffer.open(Buffer::Truncate), "[Aa]ssert");
    EXPECT_DEATH(buffer.setData({9}), "[Aa]ssert");
#endif
    EXPECT_EQ(buffer.tell(), 1);
    EXPECT_EQ(buffer.data(), ByteArray({3, 4, 5}));
    buffer.close();
    buffer.close();
    buffer.setData({9});
    buffer.open(Buffer::ReadOnly);
    EXPECT_EQ(buffer.tell(), 0);
    EXPECT_EQ(buffer.readAll(), ByteArray({9}));
}

TEST(BaseBuffer, HandlesDirectStorageChangesPastThePosition) {
    Buffer buffer({1, 2, 3});
    buffer.open(Buffer::ReadWrite);
    buffer.seek(3);
    buffer.buffer().resize(1);
    EXPECT_TRUE(buffer.readAll().empty());
    EXPECT_EQ(buffer.tell(), 3);
    buffer.writeAll(ByteArray{9});
    EXPECT_EQ(buffer.data(), ByteArray({1, 0, 0, 9}));
}

TEST(BaseBuffer, HandlesOverlappingTransfersAndSelfAppendWithReallocation) {
    Buffer buffer({1, 2, 3, 4});
    buffer.open(Buffer::ReadWrite);
    buffer.seek(1);
    EXPECT_EQ(buffer.write(std::span(buffer.data()).first(3)), 3);
    EXPECT_EQ(buffer.data(), ByteArray({1, 1, 2, 3}));
    buffer.seek(0);
    EXPECT_EQ(buffer.read(std::span(buffer.buffer()).subspan(1)), 3);
    EXPECT_EQ(buffer.data(), ByteArray({1, 1, 1, 2}));
    buffer.close();

    buffer.buffer().resize(buffer.buffer().capacity());
    const auto before = buffer.data();
    buffer.open(Buffer::Append);
    EXPECT_EQ(buffer.write(buffer.data()), before.size());
    ByteArray doubled = before;
    doubled.insert(doubled.end(), before.begin(), before.end());
    EXPECT_EQ(buffer.data(), doubled);
    EXPECT_EQ(buffer.tell(), doubled.size());
}

} // namespace
