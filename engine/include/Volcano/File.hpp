//
//
#ifndef VOLCANO_FILE_HPP
#define VOLCANO_FILE_HPP

#include <string>

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

class File: public IO {
public:
    File(void);
    File(const std::string &filename);
    ~File(void) override;

public:
    bool open(int flags) override;
    void close(void) override;
    int64_t size(void) override;
    int64_t read(void *p, int64_t size) override;
    int64_t write(const void *p, int64_t size) override;
    const std::string &filename(void) const;
    void setFilename(const std::string &r);

protected:
    int64_t pos(void) override;
    int64_t setPos(int64_t pos) override;

private:
    std::string m_filename;
    SDL_RWops *m_fops;
};

VOLCANO_INLINE const std::string &File::filename(void) const
{
    return m_filename;
}

VOLCANO_INLINE void File::setFilename(const std::string &r)
{
    VOLCANO_ASSERT(m_fops == nullptr);

    m_filename = r;
}

VOLCANO_END

#endif // VOLCANO_FILE_HPP
