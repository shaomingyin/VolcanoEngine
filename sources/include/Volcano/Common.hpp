//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <list>
#include <vector>
#include <string>
#include <cassert>
#include <cstdint>

#include <rttr/type>
#include <rttr/registration>

#include <sigslot/signal.hpp>

#include <Volcano/Common.h>

#ifdef VOLCANO_DEBUG
#   ifdef EIGEN_NO_DEBUG
#       undef EIGEN_NO_DEBUG
#   endif
#else
#   ifndef EIGEN_NO_DEBUG
#       define EIGEN_NO_DEBUG
#   endif
#endif

#include <Eigen/Core>
#include <Eigen/Geometry>


#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

enum class ByteOrder {
    BigEndian = 0,
    LittleEndian
};

using ByteArray = std::vector<uint8_t>;
using StringList = std::list<std::string>;

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
