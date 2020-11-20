//
//
#ifndef VOLCANO_UPDATER_HPP
#define VOLCANO_UPDATER_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Object.hpp>

VOLCANO_BEGIN

class Updater: public Object {
public:
    Updater(Callback cb, Object *parent = nullptr);
    ~Updater(void) override;

protected:
    Duration onUpdate(void) override;

private:
    Callback m_cb;
};

VOLCANO_END

#endif // VOLCANO_UPDATER_HPP
