#include <Volcano/Stream.h>

#include <algorithm>
#include <array>
#include <limits>

#include <gtest/gtest.h>

namespace {

using Volcano::ByteArray;
using Volcano::Stream;

class SequentialStream : public Stream {
public:
    using Stream::read;
    using Stream::write;

    bool isReadable() const override { return true; }
    bool isWritable() const override { return true; }

    std::size_t read(std::span<std::uint8_t> buffer) override {
        const auto count = std::min({buffer.size(), input.size() - position, std::size_t(2)});
        std::copy_n(input.begin() + position, count, buffer.begin());
        position += count;
        return count;
    }

    std::size_t write(std::span<const std::uint8_t> buffer) override {
        const auto count = std::min(buffer.size(), std::size_t(2));
        output.insert(output.end(), buffer.begin(), buffer.begin() + count);
        return count;
    }

    ByteArray input{1, 0, 2, 3, 4};
    ByteArray output;
    std::size_t position = 0;
};

TEST(BaseStream, SequentialReadAllStartsAtCurrentPosition) {
    SequentialStream stream;
    EXPECT_TRUE(stream.isSequential());
    EXPECT_TRUE(stream.isReadable());
    EXPECT_TRUE(stream.isWritable());
    EXPECT_THROW(stream.seek(0), std::runtime_error);
    EXPECT_THROW(stream.tell(), std::runtime_error);
    EXPECT_THROW(stream.size(), std::runtime_error);

    std::array<std::uint8_t, 1> first{};
    EXPECT_EQ(stream.read(first), 1);
    EXPECT_EQ(first[0], 1);
    EXPECT_EQ(stream.readAll(), ByteArray({0, 2, 3, 4}));
    EXPECT_TRUE(stream.readAll().empty());
}

TEST(BaseStream, ReadAllCrossesInternalBufferBoundaries) {
    SequentialStream stream;
    stream.input.resize(10000, 42);
    EXPECT_EQ(stream.readAll(), stream.input);
}

TEST(BaseStream, ReadExactHandlesShortReadsAndEarlyEof) {
    SequentialStream stream;
    ByteArray result(stream.input.size());
    stream.readExact(result);
    EXPECT_EQ(result, stream.input);

    std::array<std::uint8_t, 1> next{};
    EXPECT_THROW(stream.readExact(next), std::runtime_error);
    stream.position = 0;
    result.resize(stream.input.size() + 1);
    EXPECT_THROW(stream.readExact(result), std::runtime_error);
    EXPECT_TRUE(std::equal(stream.input.begin(), stream.input.end(), result.begin()));
}

TEST(BaseStream, WriteAllHandlesShortWrites) {
    SequentialStream stream;
    EXPECT_EQ(stream.write(stream.input), 2);
    EXPECT_EQ(stream.output, ByteArray({1, 0}));
    stream.output.clear();
    stream.writeAll(stream.input);
    EXPECT_EQ(stream.output, stream.input);
    EXPECT_NO_THROW(stream.flush());
}

TEST(BaseStream, PointerOverloadsValidateBuffersAndReturnCounts) {
    SequentialStream stream;
    std::array<std::uint8_t, 3> buffer{};
    EXPECT_THROW(stream.read(nullptr, 1), std::invalid_argument);
    EXPECT_THROW(stream.write(nullptr, 1), std::invalid_argument);
    EXPECT_EQ(stream.read(nullptr, 0), 0);
    EXPECT_EQ(stream.write(nullptr, 0), 0);
    EXPECT_EQ(stream.read(buffer.data(), buffer.size()), 2);
    EXPECT_EQ(stream.write(buffer.data(), buffer.size()), 2);
    EXPECT_EQ(stream.output, ByteArray({1, 0}));
}

class UnsupportedStream final : public Stream {
public:
    bool isReadable() const override { return false; }
    bool isWritable() const override { return false; }
};

TEST(BaseStream, UnsupportedOperationsThrowAndEmptyTransfersSucceed) {
    UnsupportedStream stream;
    EXPECT_FALSE(stream.isReadable());
    EXPECT_FALSE(stream.isWritable());
    std::array<std::uint8_t, 1> buffer{};
    EXPECT_THROW(stream.read(buffer), std::runtime_error);
    EXPECT_THROW(stream.write(buffer), std::runtime_error);
    EXPECT_THROW(stream.readAll(), std::runtime_error);
    EXPECT_EQ(stream.read(nullptr, 0), 0);
    EXPECT_EQ(stream.write(nullptr, 0), 0);
    EXPECT_NO_THROW(stream.readExact({}));
    EXPECT_NO_THROW(stream.writeAll({}));
    EXPECT_NO_THROW(stream.flush());
}

class InvalidCountStream final : public SequentialStream {
public:
    std::size_t read(std::span<std::uint8_t>) override { return count; }
    std::size_t write(std::span<const std::uint8_t>) override { return count; }

    std::size_t count = 0;
};

TEST(BaseStream, HelpersRejectInvalidCountsAndStalledWrites) {
    InvalidCountStream stream;
    std::array<std::uint8_t, 1> buffer{};
    EXPECT_THROW(stream.readExact(buffer), std::runtime_error);
    EXPECT_THROW(stream.writeAll(buffer), std::runtime_error);
    EXPECT_TRUE(stream.readAll().empty());

    stream.count = std::numeric_limits<std::size_t>::max();
    EXPECT_THROW(stream.readExact(buffer), std::runtime_error);
    EXPECT_THROW(stream.writeAll(buffer), std::runtime_error);
    EXPECT_THROW(stream.readAll(), std::runtime_error);
}

class ThrowingStream final : public SequentialStream {
public:
    std::size_t read(std::span<std::uint8_t>) override {
        throw std::overflow_error("Read failed");
    }
    std::size_t write(std::span<const std::uint8_t>) override {
        throw std::overflow_error("Write failed");
    }
};

TEST(BaseStream, HelpersPropagateIoErrors) {
    ThrowingStream stream;
    std::array<std::uint8_t, 1> buffer{};
    EXPECT_THROW(stream.readExact(buffer), std::overflow_error);
    EXPECT_THROW(stream.writeAll(buffer), std::overflow_error);
    EXPECT_THROW(stream.readAll(), std::overflow_error);
}

} // namespace
