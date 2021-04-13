//
//
#ifndef VOLCANO_FRAMEWORK_CHANNEL_HPP
#define VOLCANO_FRAMEWORK_CHANNEL_HPP

#include <vector>
#include <functional>

#include <Volcano/IO.hpp>
#include <Volcano/Framework/Common.hpp>

VOLCANO_FRAMEWORK_BEGIN

class Channel: public IO {
public:

public:
    Channel(void);
    ~Channel(void) override;

public:
    bool write(std::function<bool (flexbuffers::Builder &)> writter);
    flexbuffers::Reference read(void);
    virtual void sync(void) = 0;

private:
    using Queue = std::vector<flexbuffers::Builder>;

private:
    Queue m_queue;
    size_t m_readPos;
    size_t m_writePos;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CHANNEL_HPP
