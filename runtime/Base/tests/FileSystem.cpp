#include <Volcano/NativeFileSystem.h>
#include <Volcano/VirtualFileSystem.h>

#include <algorithm>
#include <array>
#include <barrier>
#include <chrono>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <thread>
#include <type_traits>

namespace fs = std::filesystem;
namespace vfs = Volcano;
using Volcano::ByteArray;

namespace {

template<class Type>
concept Mountable = requires(Type &filesystem, std::shared_ptr<vfs::FileSystem> storage) {
    filesystem.mount("/", storage);
    filesystem.unmount("/");
};

static_assert(std::is_abstract_v<vfs::FileSystem>);
static_assert(!Mountable<vfs::FileSystem>);
static_assert(!Mountable<vfs::NativeFileSystem>);
static_assert(Mountable<vfs::VirtualFileSystem>);

template<class Function>
void fails(Function &&function) {
    EXPECT_THROW(function(), std::exception);
}

TEST(BaseFileSystem, ConcurrentMounts) {
    auto first = std::make_shared<vfs::VirtualFileSystem>();
    auto second = std::make_shared<vfs::VirtualFileSystem>();
    std::barrier start(3);
    bool firstMounted = false;
    bool secondMounted = false;
    const auto mount = [&](auto &parent, auto &child, bool &mounted) {
        start.arrive_and_wait();
        try {
            parent->mount("/child", child);
            mounted = true;
        } catch (const std::invalid_argument &) {
        }
    };
    std::thread left([&] {
        mount(first, second, firstMounted);
    });
    std::thread right([&] {
        mount(second, first, secondMounted);
    });
    start.arrive_and_wait();
    left.join();
    right.join();
    EXPECT_TRUE((firstMounted != secondMounted));
}

struct Fixture {
    fs::path root;

    Fixture() {
        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        for (int index = 0; ; ++index) {
            root = fs::temp_directory_path() / ("volcano-fs-" + std::to_string(stamp) + "-" + std::to_string(index));
            if (fs::create_directory(root)) {
                break;
            }
        }
    }

    ~Fixture() {
        std::error_code error;
        fs::remove_all(root, error);
    }

    void put(const fs::path &path, const std::string &text) {
        fs::create_directories((root / path).parent_path());
        std::ofstream output(root / path, std::ios::binary);
        output.write(text.data(), static_cast<std::streamsize>(text.size()));
        output.close();
        EXPECT_TRUE((static_cast<bool>(output)));
    }
};

std::string text(const vfs::FileSystem &filesystem, const fs::path &path) {
    const auto bytes = filesystem.readFile(path);
    return std::string(bytes.begin(), bytes.end());
}

// File handles retain their storage independently of the filesystem, as archive entries must.
class MemoryFile final: public vfs::File {
public:
    explicit MemoryFile(std::shared_ptr<const ByteArray> data) : data_(std::move(data)) {
    }

    bool isReadable() const override { return true; }
    bool isWritable() const override { return false; }

    std::size_t read(std::span<std::uint8_t> data) override {
        const auto count = std::min({data.size(), data_->size() - position_, std::size_t(2)});
        std::copy_n(data_->begin() + position_, count, data.begin());
        position_ += count;
        return count;
    }

    void seek(std::size_t position) override {
        if (position > size()) {
            throw std::out_of_range("Invalid memory file seek");
        }
        position_ = position;
    }

    std::size_t tell() const override {
        return position_;
    }
    std::size_t size() override {
        return data_->size();
    }

private:
    std::shared_ptr<const ByteArray> data_;
    std::size_t position_ = 0;
};

class MemoryFileSystem final: public vfs::FileSystem {
public:
    mutable unsigned mutationCalls = 0;
    std::weak_ptr<const ByteArray> storage() const {
        return data_;
    }

protected:
    vfs::DirEntry statImpl(const fs::path &path) const override {
        if (path.empty()) {
            return vfs::DirEntry(path, vfs::FileType::Directory);
        }
        if (path == "entry") {
            return vfs::DirEntry(path, vfs::FileType::Regular, data_->size());
        }
        return vfs::DirEntry(path);
    }

    std::vector<vfs::DirEntry> listDirectoryImpl(const fs::path &) const override {
        return {statImpl("entry")};
    }

    std::unique_ptr<vfs::File> openFileForReadImpl(const fs::path &path) const override {
        if (!statImpl(path).isFile()) {
            throw std::runtime_error("Memory file does not exist");
        }
        return std::make_unique<MemoryFile>(data_);
    }

    std::unique_ptr<vfs::File> openFileForWriteImpl(const fs::path &, bool) const override {
        ++mutationCalls;
        throw std::runtime_error("Read-only write hook must not be called");
    }

    void removeImpl(const fs::path &) const override {
        ++mutationCalls;
    }
    void createDirectoryImpl(const fs::path &) const override {
        ++mutationCalls;
    }

private:
    std::shared_ptr<const ByteArray> data_ = std::make_shared<const ByteArray>(ByteArray{'m', 'e', 'm', 'o', 'r', 'y'});
};

TEST(BaseFileSystem, Operations) {
    Fixture fixture;
    fixture.put("low/shared", "low");
    fixture.put("low/only-low", "old");
    fixture.put("low/tree/leaf", "leaf");
    fixture.put("low/blocked/child", "hidden");
    fixture.put("high/shared", "high");
    fixture.put("high/blocked", "file");
    fixture.put("high/empty", "");
    fixture.put(fs::path("high") / fs::path(u8"中文.bin"), std::string("a\0b", 3));
    fs::create_directory(fixture.root / "write");

    fixture.put("high/assets/shared", "parent");
    fixture.put("high/assets/parent-only", "hidden");
    fixture.put("high/cover", "file");
    fixture.put("high/reveal", "upper");
    fixture.put("low/reveal", "lower");
    {
        vfs::VirtualFileSystem routing;
        routing.mount("/assets", fixture.root / "low");
        routing.mount("/", fixture.root / "high");
        EXPECT_TRUE((text(routing, "/assets/shared") == "low"));
        EXPECT_TRUE((!routing.exists("/assets/parent-only")));
        routing.writeFile("/assets/routed", ByteArray{'r'});
        EXPECT_TRUE((fs::exists(fixture.root / "low/routed")));
        EXPECT_TRUE((!fs::exists(fixture.root / "high/assets/routed")));
        const auto entries = routing.listDirectory("/assets");
        EXPECT_TRUE((std::none_of(entries.begin(), entries.end(), [](const auto &entry) {
            return entry.path() == "/assets/parent-only";
        })));
        routing.mount("/assets/deep", fixture.root / "high");
        routing.mount("/cover/deep", fixture.root / "low");
        EXPECT_TRUE((routing.isDirectory("/cover")));
        EXPECT_TRUE((text(routing, "/cover/deep/shared") == "low"));
        routing.mount("/", fixture.root / "low");
        EXPECT_TRUE((text(routing, "/assets/deep/shared") == "high"));
        routing.unmount("/assets/deep");
        EXPECT_TRUE((!routing.exists("/assets/deep")));
        routing.mount("/assets", std::make_shared<MemoryFileSystem>());
        EXPECT_TRUE((!routing.isWritable("/assets/entry")));
        EXPECT_TRUE((!routing.isWritable("/assets/new-file")));
        EXPECT_TRUE((routing.isWritable("/assets/shared")));
        fails([&] {
            routing.writeFile("/assets/new-file", {});
        });
        EXPECT_TRUE((!fs::exists(fixture.root / "low/new-file")));
    }
    {
        vfs::VirtualFileSystem layers;
        layers.mount("/", fixture.root / "low");
        layers.mount("/", fixture.root / "high");
        EXPECT_TRUE((text(layers, "/reveal") == "upper"));
        layers.remove("/reveal");
        EXPECT_TRUE((text(layers, "/reveal") == "lower"));
    }
    {
        std::unique_ptr<vfs::File> direct;
        std::weak_ptr<const ByteArray> storage;
        {
            MemoryFileSystem memory;
            storage = memory.storage();
            direct = memory.openFileForRead("/entry");
        }
        EXPECT_TRUE((!storage.expired()));
        EXPECT_TRUE((direct->read() == ByteArray({'m', 'e', 'm', 'o', 'r', 'y'})));
        direct.reset();
        EXPECT_TRUE((storage.expired()));
    }

    auto owner = std::make_unique<vfs::VirtualFileSystem>();
    auto &filesystem = *owner;
    vfs::VirtualFileSystem independent;
    EXPECT_TRUE((independent.isDirectory("/") && independent.listDirectory("/").empty()));
    EXPECT_TRUE((filesystem.isDirectory("/")));
    EXPECT_TRUE((filesystem.listDirectory("/").empty()));
    fails([&] {
        filesystem.mount("/", fixture.root / "absent");
    });
    fails([&] {
        filesystem.mount("/", std::shared_ptr<vfs::FileSystem>{});
    });
    filesystem.mount("/", fixture.root / "high");
    filesystem.mount("/", fixture.root / "low", true);
    EXPECT_TRUE((text(filesystem, "/shared") == "high"));
    EXPECT_TRUE((text(filesystem, "/tree/../shared") == "high"));
    EXPECT_TRUE((text(filesystem, "/tree//./../shared") == "high"));
    EXPECT_TRUE((filesystem.isDirectory("/tree/..")));
    EXPECT_TRUE((filesystem.stat("/tree/../shared").path() == "/shared"));
    EXPECT_TRUE((text(filesystem, "/only-low") == "old"));
    EXPECT_TRUE((filesystem.readFile("/empty").empty()));
    EXPECT_TRUE((filesystem.readFile(fs::path(u8"/中文.bin")) == ByteArray({'a', 0, 'b'})));
    EXPECT_TRUE((vfs::FileSystem::toNativeString(fs::path(u8"中文.bin")) == "中文.bin"));
    EXPECT_TRUE((!filesystem.exists("/blocked/child")));
    fails([&] {
        filesystem.openFileForRead("/blocked/child");
    });
    fails([&] {
        filesystem.listDirectory("/blocked");
    });
    EXPECT_TRUE((!filesystem.stat("/absent").exists()));
    EXPECT_TRUE((!filesystem.isFile("/absent")));
    fails([&] {
        filesystem.fileSize("/");
    });
    fails([&] {
        filesystem.openFileForRead("/");
    });
    fails([&] {
        filesystem.readFile("/absent");
    });

    const auto entries = filesystem.listDirectory("/");
    EXPECT_TRUE((std::count_if(entries.begin(), entries.end(), [](const auto &entry) {
        return entry.path() == "/shared" && entry.isFile() && entry.size() == 4;
    }) == 1));
    EXPECT_TRUE((std::is_sorted(entries.begin(), entries.end(), [](const auto &a, const auto &b) {
        return a.path() < b.path();
    })));
    auto file = filesystem.openFileForRead("/shared");
    EXPECT_TRUE(file->isReadable());
    EXPECT_FALSE(file->isWritable());
    EXPECT_FALSE(file->isSequential());
    std::array<std::uint8_t, 8> buffer{};
    EXPECT_TRUE((file->size() == 4 && file->tell() == 0));
    EXPECT_TRUE((file->read(buffer) == 4 && file->tell() == 4));
    EXPECT_TRUE((file->read(buffer) == 0));
    EXPECT_TRUE((file->size() == 4 && file->tell() == 4));
    file->seek(1);
    EXPECT_TRUE((file->read(buffer) == 3 && buffer[0] == 'i'));
    fails([&] {
        file->seek(5);
    });
    fails([&] {
        file->read(nullptr, 1);
    });
    fails([&] {
        file->write(buffer);
    });
    EXPECT_TRUE((file->read(nullptr, 0) == 0));
    EXPECT_EQ(file->write(nullptr, 0), 0);
    file->seek(1);
    EXPECT_EQ(file->readAll(), ByteArray({'i', 'g', 'h'}));
    EXPECT_TRUE((file->read() == ByteArray({'h', 'i', 'g', 'h'})));

    const ByteArray first{'1', 0, '2'};
    EXPECT_TRUE((!independent.isWritable("/new")));
    fails([&] {
        independent.writeFile("/file", first);
    });
    fails([&] {
        independent.createDirectory("/directory");
    });
    fails([&] {
        independent.remove("/file");
    });
    filesystem.mount("/output", fixture.root / "write");
    filesystem.createDirectory("/output/unused/../nested/deep");
    filesystem.writeFile("/output/nested/unused/../deep/file", first);
    EXPECT_TRUE((filesystem.readFile("/output/nested/deep/file") == first));
    filesystem.appendFile("/output/nested/../nested/deep/file", ByteArray{'3'});
    EXPECT_TRUE((filesystem.fileSize("/output/nested/deep/file") == 4));
    filesystem.writeFile("/output/nested/deep/file", ByteArray{'x'});
    EXPECT_TRUE((text(filesystem, "/output/nested/deep/file") == "x"));
    auto output = filesystem.openFileForAppend("/output/nested/deep/file");
    EXPECT_FALSE(output->isReadable());
    EXPECT_TRUE(output->isWritable());
    EXPECT_FALSE(output->isSequential());
    EXPECT_EQ(output->read(nullptr, 0), 0);
    EXPECT_EQ(output->write(nullptr, 0), 0);
    output->seek(0);
    EXPECT_EQ(output->write(ByteArray{'y'}), 1);
    output->flush();
    EXPECT_TRUE((output->tell() == 2 && text(filesystem, "/output/nested/deep/file") == "xy"));
    fails([&] {
        output->read(buffer);
    });
    fails([&] {
        output->write(nullptr, 1);
    });
    output.reset();
    fails([&] {
        filesystem.remove("/output/nested");
    });
    filesystem.remove("/output/nested/deep/../deep/file");
    EXPECT_TRUE((!filesystem.exists("/output/nested/deep/file")));
    filesystem.remove("/output/nested/deep");
    fails([&] {
        filesystem.remove("/output/missing");
    });
    fails([&] {
        filesystem.writeFile("/output/absent/file", {});
    });
    fails([&] {
        filesystem.writeFile("/output/nested", {});
    });
    fails([&] {
        filesystem.remove("/");
    });
    fails([&] {
        filesystem.openFileForWrite("/");
    });
    fails([&] {
        filesystem.remove("/output/..");
    });
    EXPECT_TRUE((text(filesystem, "/output/../shared") == "high"));
    fails([&] {
        filesystem.remove("/output");
    });
    fails([&] {
        filesystem.createDirectory("/output");
    });
    fails([&] {
        filesystem.writeFile("/blocked/child", {});
    });
    fails([&] {
        filesystem.createDirectory("/blocked/child");
    });
    fails([&] {
        filesystem.remove("/blocked/child");
    });
    filesystem.writeFile("/created", ByteArray{'n'});
    EXPECT_TRUE((fs::exists(fixture.root / "high/created")));
    EXPECT_TRUE((!fs::exists(fixture.root / "low/created")));
    filesystem.appendFile("/only-low", ByteArray{'!'});
    EXPECT_TRUE((text(filesystem, "/only-low") == "old!"));
    EXPECT_TRUE((!fs::exists(fixture.root / "high/only-low")));

    filesystem.mount("/unused/../override", fixture.root / "low");
    filesystem.mount("/override", fixture.root / "high");
    EXPECT_TRUE((text(filesystem, "/override/shared") == "high"));
    filesystem.unmount("/override/child/..");
    EXPECT_TRUE((!filesystem.exists("/override")));
    fails([&] {
        filesystem.unmount("/override");
    });

    filesystem.mount("/virtual/assets", fixture.root / "low");
    EXPECT_TRUE((filesystem.isDirectory("/virtual") && filesystem.isDirectory("/virtual/assets")));
    EXPECT_TRUE((text(filesystem, "/virtual/assets/shared") == "low"));
    EXPECT_TRUE((!filesystem.exists("/virtual/assets2/shared")));
    filesystem.writeFile("/virtual/assets/new", ByteArray{'v'});
    EXPECT_TRUE((fs::exists(fixture.root / "low/new")));
    fails([&] {
        filesystem.writeFile("/virtual/assets2/new", {});
    });
    fails([&] {
        filesystem.remove("/virtual");
    });
    fails([&] {
        filesystem.remove("/virtual/assets");
    });
    const auto recursive = filesystem.listDirectory("/virtual", true);
    EXPECT_TRUE((std::any_of(recursive.begin(), recursive.end(), [](const auto &entry) {
        return entry.path() == "/virtual/assets/tree/leaf";
    })));
    int visited = 0;
    filesystem.listDirectory("/", [&](const vfs::DirEntry &entry) {
        EXPECT_TRUE((filesystem.exists(entry.path())));
        ++visited;
        return false;
    }, true);
    EXPECT_TRUE((visited == 1));
    fails([&] {
        filesystem.listDirectory("/", vfs::FileSystem::DirectoryCallback{});
    });

    for (const auto &invalid : {fs::path{}, fs::path("."), fs::path("shared"),
             fs::path("../escape"), fs::path("a/../shared"), fs::path("/../escape"),
             fs::path("/a/../../escape"), fs::path("/a/../.."), fs::path("C:/file"),
             fs::path("/C:/file"), fs::path("/a\\file"), fs::path(std::string("/bad\0name", 9))}) {
        fails([&] {
            filesystem.isWritable(invalid);
        });
        fails([&] {
            filesystem.stat(invalid);
        });
        fails([&] {
            filesystem.exists(invalid);
        });
        fails([&] {
            filesystem.isFile(invalid);
        });
        fails([&] {
            filesystem.isDirectory(invalid);
        });
        fails([&] {
            filesystem.fileSize(invalid);
        });
        fails([&] {
            filesystem.readFile(invalid);
        });
        fails([&] {
            filesystem.openFileForWrite(invalid);
        });
        fails([&] {
            filesystem.openFileForAppend(invalid);
        });
        fails([&] {
            filesystem.remove(invalid);
        });
        fails([&] {
            filesystem.createDirectory(invalid);
        });
        fails([&] {
            filesystem.listDirectory(invalid);
        });
        fails([&] {
            filesystem.mount(invalid, fixture.root / "low");
        });
        fails([&] {
            filesystem.unmount(invalid);
        });
    }
    vfs::NativeFileSystem native(fixture.root / "high");
    EXPECT_TRUE((native.isWritable("/direct")));
    fails([&] {
        native.stat("/../low/shared");
    });
    EXPECT_TRUE((native.readFile("/shared") == ByteArray({'h', 'i', 'g', 'h'})));
    fails([&] {
        native.remove("/.");
    });
    native.writeFile("/direct", ByteArray{'d'});
    EXPECT_TRUE((native.readFile("/direct") == ByteArray{'d'}));
    native.appendFile("/direct", ByteArray{'e'});
    EXPECT_TRUE((native.readFile("/direct") == ByteArray({'d', 'e'})));
    native.remove("/direct");
    vfs::VirtualFileSystem nativeView;
    nativeView.mount("/", fixture.root / "high");
    nativeView.mount("/mounted", fixture.root / "low");
    EXPECT_TRUE((nativeView.readFile("/mounted/shared") == ByteArray({'l', 'o', 'w'})));
    nativeView.writeFile("/mounted/native-write", ByteArray{'w'});
    EXPECT_TRUE((fs::exists(fixture.root / "low/native-write")));
    EXPECT_TRUE((!fs::exists(fixture.root / "high/mounted")));
    std::unique_ptr<vfs::File> detached;
    {
        vfs::NativeFileSystem temporary(fixture.root / "high");
        detached = temporary.openFileForRead("/shared");
    }
    EXPECT_TRUE((detached->read() == ByteArray({'h', 'i', 'g', 'h'})));

    auto parent = std::make_shared<vfs::VirtualFileSystem>();
    auto child = std::make_shared<vfs::VirtualFileSystem>();
    child->mount("/disk", fixture.root / "low");
    parent->mount("/child", child);
    filesystem.mount("/nested", parent);
    EXPECT_TRUE((text(filesystem, "/nested/child/disk/shared") == "low"));
    EXPECT_TRUE((filesystem.listDirectory("/nested", true).size() == parent->listDirectory("/", true).size()));
    fails([&] {
        parent->mount("/self", parent);
    });
    fails([&] {
        child->mount("/cycle", parent);
    });
    filesystem.writeFile("/nested/child/disk/nested-write", ByteArray{'w'});
    EXPECT_TRUE((fs::exists(fixture.root / "low/nested-write")));
    fails([&] {
        filesystem.writeFile("/nested/child/no-storage", {});
    });
    EXPECT_TRUE((independent.listDirectory("/").empty()));
    std::error_code error;
    fs::create_directory_symlink(fixture.root / "low", fixture.root / "high/link", error);
    if (!error) {
        fails([&] {
            filesystem.readFile("/link/shared");
        });
        const auto safeEntries = native.listDirectory("/");
        EXPECT_TRUE((std::none_of(safeEntries.begin(), safeEntries.end(), [](const auto &entry) {
            return entry.path() == "/link";
        })));
        fails([&] {
            filesystem.writeFile("/link/shared", {});
        });
        EXPECT_TRUE((text(filesystem, "/virtual/assets/shared") == "low"));
    } else {
        std::cout << "Symbolic link checks skipped: " << error.message() << '\n';
    }

    fixture.put("low/entry", "lower");
    auto memory = std::make_shared<MemoryFileSystem>();
    std::weak_ptr<vfs::FileSystem> lifetime = memory;
    auto storage = memory->storage();
    filesystem.mount("/memory", memory);
    filesystem.mount("/memory", fixture.root / "low", true);
    EXPECT_TRUE((!memory->isWritable("/entry")));
    EXPECT_TRUE((text(filesystem, "/memory/entry") == "memory"));
    EXPECT_TRUE((!filesystem.isWritable("/memory/entry")));
    EXPECT_TRUE((!filesystem.isWritable("/memory/missing-entry")));
    EXPECT_TRUE((filesystem.isWritable("/memory/only-low")));
    for (const auto &path : {"/entry", "/missing-entry"}) {
        fails([&] {
            memory->writeFile(path, {});
        });
        fails([&] {
            memory->appendFile(path, {});
        });
        fails([&] {
            memory->remove(path);
        });
        fails([&] {
            memory->createDirectory(path);
        });
        const auto mounted = fs::path("/memory") / fs::path(path).relative_path();
        fails([&] {
            filesystem.writeFile(mounted, {});
        });
        fails([&] {
            filesystem.appendFile(mounted, {});
        });
        fails([&] {
            filesystem.remove(mounted);
        });
        fails([&] {
            filesystem.createDirectory(mounted);
        });
    }
    EXPECT_TRUE((memory->mutationCalls == 0));
    EXPECT_TRUE((text(vfs::NativeFileSystem(fixture.root / "low"), "/entry") == "lower"));
    EXPECT_TRUE((!fs::exists(fixture.root / "low/missing-entry")));
    filesystem.appendFile("/memory/only-low", ByteArray{'!'});
    EXPECT_TRUE((text(filesystem, "/memory/only-low") == "old!!"));
    auto borrowed = filesystem.openFileForRead("/memory/entry");
    memory.reset();
    filesystem.unmount("/memory");
    owner.reset();
    EXPECT_TRUE((lifetime.expired()));
    EXPECT_TRUE((!storage.expired()));
    EXPECT_TRUE((borrowed->read() == ByteArray({'m', 'e', 'm', 'o', 'r', 'y'})));
    EXPECT_TRUE((file->read() == ByteArray({'h', 'i', 'g', 'h'})));
    borrowed.reset();
    EXPECT_TRUE((storage.expired()));
    EXPECT_TRUE((independent.listDirectory("/").empty()));
}

struct WriteState {
    ByteArray bytes;
    int flushes = 0;
    bool failFlush = false;
};

class ShortWriteFile final : public vfs::File {
public:
    explicit ShortWriteFile(std::shared_ptr<WriteState> state)
        : state_(std::move(state)), position_(state_->bytes.size()) {
    }

    bool isReadable() const override { return false; }
    bool isWritable() const override { return true; }
    std::size_t tell() const override { return position_; }
    std::size_t size() override { return state_->bytes.size(); }

    void seek(std::size_t position) override {
        if (position > size()) {
            throw std::out_of_range("Seek past end");
        }
        position_ = position;
    }

    std::size_t write(std::span<const std::uint8_t> data) override {
        const auto count = std::min(data.size(), std::size_t(2));
        state_->bytes.resize(std::max(size(), position_ + count));
        std::copy_n(data.begin(), count, state_->bytes.begin() + position_);
        position_ += count;
        return count;
    }

    void flush() override {
        ++state_->flushes;
        if (state_->failFlush) {
            throw std::runtime_error("Flush failed");
        }
    }

private:
    std::shared_ptr<WriteState> state_;
    std::size_t position_;
};

class ShortWriteFileSystem final : public vfs::FileSystem {
public:
    std::shared_ptr<WriteState> state = std::make_shared<WriteState>();

protected:
    bool isWritableImpl(const fs::path &) const override { return true; }

    vfs::DirEntry statImpl(const fs::path &path) const override {
        return vfs::DirEntry(path, path.empty() ? vfs::FileType::Directory : vfs::FileType::Regular,
            path.empty() ? 0 : state->bytes.size());
    }

    std::vector<vfs::DirEntry> listDirectoryImpl(const fs::path &) const override {
        return {statImpl("entry")};
    }

    std::unique_ptr<vfs::File> openFileForReadImpl(const fs::path &) const override {
        throw std::runtime_error("Write-only storage");
    }

    std::unique_ptr<vfs::File> openFileForWriteImpl(const fs::path &, bool append) const override {
        if (!append) {
            state->bytes.clear();
        }
        return std::make_unique<ShortWriteFile>(state);
    }
};

TEST(BaseFileSystem, WriteAndAppendCompleteShortWritesAndFlush) {
    ShortWriteFileSystem filesystem;
    const ByteArray data{1, 0, 2, 3, 4};
    filesystem.writeFile("/entry", data);
    EXPECT_EQ(filesystem.state->bytes, data);
    EXPECT_EQ(filesystem.state->flushes, 1);

    filesystem.appendFile("/entry", data);
    ByteArray doubled = data;
    doubled.insert(doubled.end(), data.begin(), data.end());
    EXPECT_EQ(filesystem.state->bytes, doubled);
    EXPECT_EQ(filesystem.state->flushes, 2);

    filesystem.writeFile("/entry", {});
    EXPECT_TRUE(filesystem.state->bytes.empty());
    EXPECT_EQ(filesystem.state->flushes, 3);
    filesystem.state->failFlush = true;
    EXPECT_THROW(filesystem.writeFile("/entry", data), std::runtime_error);
    EXPECT_THROW(filesystem.appendFile("/entry", data), std::runtime_error);
}

} // namespace
