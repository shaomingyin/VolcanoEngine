//
//
#ifndef VOLCANO_VERSIONNUMBER_H
#define VOLCANO_VERSIONNUMBER_H

#include <Volcano/Common.h>

VOLCANO_BEGIN

class VersionNumber final {
public:
    VersionNumber(int major, int minor, int patch)
        : major_(major)
        , minor_(minor)
        , patch_(patch) {
    }

public:

private:
    int major_;
    int minor_;
    int patch_;
};

extern const VersionNumber Version;

VOLCANO_END

#endif // VOLCANO_VERSIONNUMBER_H
