//
//
#include <Volcano/File.hpp>

VOLCANO_BEGIN

File::File(void):
    m_fops(nullptr)
{
}

File::File(const std::string &filename) :
    m_filename(filename),
    m_fops(nullptr)
{
}

File::~File(void)
{
    if (isOpen())
        close();
}

bool File::open(int flags)
{
    VOLCANO_ASSERT(m_fops == nullptr);

    if (!IO::open(flags))
        return false;

    std::string mode;

    if (flags & FLAG_READ)
        mode += 'r';
    if (flags & FLAG_WRITE)
        mode += 'w';

    m_fops = SDL_RWFromFile(m_filename.c_str(), mode.c_str());
    if (m_fops == nullptr) {
        IO::close();
        return false;
    }

    setSeekable(true);

    return true;
}

void File::close(void)
{
    VOLCANO_ASSERT(m_fops != nullptr);

    SDL_RWclose(m_fops);
    m_fops = nullptr;

    IO::close();
}

int64_t File::size(void)
{
    VOLCANO_ASSERT(m_fops != nullptr);

    return SDL_RWsize(m_fops);
}

int64_t File::read(void *p, int64_t size)
{
    VOLCANO_ASSERT(m_fops != nullptr);

    return SDL_RWread(m_fops, p, 1, size);
}

int64_t File::write(const void *p, int64_t size)
{
    VOLCANO_ASSERT(m_fops != nullptr);

    return SDL_RWwrite(m_fops, p, 1, size);
}

int64_t File::pos(void)
{
    VOLCANO_ASSERT(m_fops != nullptr);

    return SDL_RWtell(m_fops);
}

int64_t File::setPos(int64_t pos)
{
    VOLCANO_ASSERT(m_fops != nullptr);

    return SDL_RWseek(m_fops, pos, RW_SEEK_SET);
}

VOLCANO_END
