#include <Volcano/VirtualFileSystem.h>
#include <Volcano/NativeFileSystem.h>

#include <algorithm>
#include <iterator>
#include <set>
#include <stdexcept>
#include <utility>

VOLCANO_BEGIN

namespace fs = std::filesystem;

// Serialize graph updates so concurrent mounts cannot introduce an ownership cycle.
std::mutex VirtualFileSystem::topologyMutex_;

const VirtualFileSystem::Mount *VirtualFileSystem::selectMount(const State &view, const fs::path &path) {
    for (const auto &mount : view.mounts) {
        if (isPrefix(mount.point, path)) {
            return &mount;
        }
    }
    return nullptr;
}

VirtualFileSystem::State VirtualFileSystem::snapshot() const {
    std::lock_guard lock(stateMutex_);
    return state_;
}

bool VirtualFileSystem::reaches(const VirtualFileSystem *filesystem) const {
    std::vector<const VirtualFileSystem *> pending{this};
    std::set<const VirtualFileSystem *> visited;
    while (!pending.empty()) {
        const auto current = pending.back();
        pending.pop_back();
        if (current == filesystem) {
            return true;
        }
        if (!visited.insert(current).second) {
            continue;
        }
        const auto view = current->snapshot();
        for (const auto &mount : view.mounts) {
            if (const auto child = dynamic_cast<const VirtualFileSystem *>(mount.filesystem.get())) {
                pending.push_back(child);
            }
        }
    }
    return false;
}

VirtualFileSystem::Lookup VirtualFileSystem::lookup(const State &view, const fs::path &path) const {
    for (const auto &mount : view.mounts) {
        if (path != mount.point && isPrefix(path, mount.point)) {
            return {DirEntry(path, FileType::Directory), {}, {}};
        }
    }
    const auto selected = selectMount(view, path);
    if (selected) {
        const auto relative = relativeTo(path, selected->point);
        for (const auto &mount : view.mounts) {
            if (mount.point == selected->point) {
                const auto entry = mount.filesystem->stat(virtualPath(relative));
                if (entry.exists()) {
                    return {DirEntry(path, entry.type(), entry.isFile() ? entry.size() : 0), mount.filesystem, relative};
                }
            }
        }
        // Missing entries still belong to the highest layer at the selected mount point.
        return {DirEntry(path), selected->filesystem, relative};
    }
    return {DirEntry(path, path.empty() ? FileType::Directory : FileType::Missing), {}, path};
}

VirtualFileSystem::Lookup VirtualFileSystem::visible(const State &view, const fs::path &path) const {
    for (auto parent = path.parent_path(); !parent.empty(); parent = parent.parent_path()) {
        if (!lookup(view, parent).entry.isDirectory()) {
            return {DirEntry(path), {}, path};
        }
    }
    return lookup(view, path);
}

VirtualFileSystem::Lookup VirtualFileSystem::writable(const State &view, const fs::path &path) const {
    for (const auto &mount : view.mounts) {
        if (isPrefix(path, mount.point)) {
            return {DirEntry(path), {}, path};
        }
    }
    for (auto parent = path.parent_path(); !parent.empty(); parent = parent.parent_path()) {
        const auto entry = lookup(view, parent).entry;
        if (entry.exists() && !entry.isDirectory()) {
            return {DirEntry(path), {}, path};
        }
    }
    return lookup(view, path);
}

std::vector<DirEntry> VirtualFileSystem::children(const State &view, const fs::path &path) const {
    if (!visible(view, path).entry.isDirectory()) {
        throw std::runtime_error("Cannot enumerate non-directory: " + virtualPath(path).generic_string());
    }
    std::set<fs::path> names;
    const auto selected = selectMount(view, path);
    for (const auto &mount : view.mounts) {
        if (selected && mount.point == selected->point) {
            const auto relative = relativeTo(path, mount.point);
            if (!mount.filesystem->isDirectory(virtualPath(relative))) {
                continue;
            }
            for (const auto &entry : mount.filesystem->listDirectory(virtualPath(relative))) {
                const auto child = normalizePath(entry.path());
                if (child.parent_path() != relative) {
                    throw std::runtime_error("Mounted filesystem returned an invalid child path");
                }
                names.insert(child.filename());
            }
        } else if (path != mount.point && isPrefix(path, mount.point)) {
            names.insert(*relativeTo(mount.point, path).begin());
        }
    }
    std::vector<DirEntry> result;
    for (const auto &name : names) {
        const auto entry = visible(view, path / name).entry;
        if (entry.exists()) {
            result.emplace_back(name, entry.type(), entry.isFile() ? entry.size() : 0);
        }
    }
    return result;
}

void VirtualFileSystem::mount(const fs::path &path, const fs::path &osPath, bool append) {
    mount(path, std::make_shared<NativeFileSystem>(osPath), append);
}

void VirtualFileSystem::mount(const fs::path &path, std::shared_ptr<FileSystem> filesystem, bool append) {
    const auto point = normalizePath(path);
    if (!filesystem || !filesystem->isDirectory("/")) {
        throw std::invalid_argument("Mounted filesystem must have a directory root");
    }
    Mount entry{point, std::move(filesystem)};
    std::lock_guard topologyLock(topologyMutex_);
    if (const auto child = dynamic_cast<const VirtualFileSystem *>(entry.filesystem.get()); child && child->reaches(this)) {
        throw std::invalid_argument("Filesystem mount would create a cycle");
    }
    std::lock_guard lock(stateMutex_);
    if (append) {
        state_.mounts.push_back(std::move(entry));
    } else {
        state_.mounts.insert(state_.mounts.begin(), std::move(entry));
    }
    std::stable_sort(state_.mounts.begin(), state_.mounts.end(), [](const Mount &left, const Mount &right) {
        return std::distance(left.point.begin(), left.point.end()) > std::distance(right.point.begin(), right.point.end());
    });
}

void VirtualFileSystem::unmount(const fs::path &path) {
    const auto point = normalizePath(path);
    State retired;
    {
        std::lock_guard topologyLock(topologyMutex_);
        std::lock_guard lock(stateMutex_);
        // Release filesystem objects after unlocking to allow destructor callbacks.
        retired = state_;
        const auto count = std::erase_if(state_.mounts, [&](const Mount &entry) {
            return entry.point == point;
        });
        if (count == 0) {
            throw std::runtime_error("Mount point does not exist: " + virtualPath(point).generic_string());
        }
    }
}

bool VirtualFileSystem::isWritableImpl(const fs::path &path) const {
    const auto found = writable(snapshot(), path);
    return found.filesystem && found.filesystem->isWritable(virtualPath(found.relative));
}

DirEntry VirtualFileSystem::statImpl(const fs::path &path) const {
    return visible(snapshot(), path).entry;
}

std::vector<DirEntry> VirtualFileSystem::listDirectoryImpl(const fs::path &path) const {
    return children(snapshot(), path);
}

std::unique_ptr<File> VirtualFileSystem::openFileForReadImpl(const fs::path &path) const {
    const auto found = visible(snapshot(), path);
    if (!found.filesystem || !found.entry.isFile()) {
        throw std::runtime_error("Cannot read non-regular file: " + virtualPath(path).generic_string());
    }
    return found.filesystem->openFileForRead(virtualPath(found.relative));
}

std::unique_ptr<File> VirtualFileSystem::openFileForWriteImpl(const fs::path &path, bool append) const {
    const auto found = writable(snapshot(), path);
    if (!found.filesystem) {
        throw std::runtime_error("No filesystem accepts modification: " + virtualPath(path).generic_string());
    }
    return append ? found.filesystem->openFileForAppend(virtualPath(found.relative))
                  : found.filesystem->openFileForWrite(virtualPath(found.relative));
}

void VirtualFileSystem::removeImpl(const fs::path &path) const {
    const auto found = writable(snapshot(), path);
    if (!found.filesystem) {
        throw std::runtime_error("No filesystem accepts modification: " + virtualPath(path).generic_string());
    }
    found.filesystem->remove(virtualPath(found.relative));
}

void VirtualFileSystem::createDirectoryImpl(const fs::path &path) const {
    const auto found = writable(snapshot(), path);
    if (!found.filesystem) {
        throw std::runtime_error("No filesystem accepts modification: " + virtualPath(path).generic_string());
    }
    found.filesystem->createDirectory(virtualPath(found.relative));
}

VOLCANO_END
