//
//
#ifndef VOLCANO_PARCELABLE_HPP
#define VOLCANO_PARCELABLE_HPP

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Parcel;

class Parcelable {
public:
    Parcelable(void) = default;
    virtual ~Parcelable(void) = default;

public:
    virtual bool marshalling(Parcel &parcel) = 0;
    virtual bool unmarshalling(Parcel &parcel) = 0;
};

VOLCANO_END

#endif // VOLCANO_PARCELABLE_HPP
