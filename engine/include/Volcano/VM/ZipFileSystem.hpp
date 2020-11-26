//
//
#ifndef VOLCANO_VM_ZIPFILESYSTEM_HPP
#define VOLCANO_VM_ZIPFILESYSTEM_HPP

#include <memory>
#include <string>
#include <map>

#include <Volcano/ZipReader.hpp>
#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/FileSystem.hpp>

VOLCANO_VM_BEGIN

typedef std::shared_ptr<ZipReader> ZipReaderPtr;

class ZipFileSystem: public FileSystem {
public:
    ZipFileSystem(void);
    ~ZipFileSystem(void) override;

public:
    bool isWritable(void) override;
    bool init(const std::string &native_path) override;
    void shutdown(void) override;
    Type type(const std::string &path) override;
    bool enumDir(const std::string &path, StringList &result) override;
    bool makeDir(const std::string &path) override;
    IO *openFile(const std::string &path, int io_flags) override;
    bool remove(const std::string &path) override;

protected:
    struct Node {
        Node(void): firstChild(-1), nextSibling(-1) { }

        std::string name;
        int firstChild;
        int nextSibling;
    };

    bool populateNode(const ZipReader::Stat &st);
    int findChild(const std::string &name, int parent);
    bool enumChildren(int parent, StringList &result, bool recursive);

private:
    ZipReaderPtr m_reader;
    std::vector<Node> m_tree;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_ZIPFILESYSTEM_HPP
