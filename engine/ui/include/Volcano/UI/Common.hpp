//
//
#ifndef VOLCANO_UI_COMMON_HPP
#define VOLCANO_UI_COMMON_HPP

#include <Volcano/Common.hpp>

#ifdef VOLCANO_UI
#   define VOLCANO_UI_API Q_DECL_EXPORT
#else
#   define VOLCANO_UI_API
#endif

#define VOLCANO_UI_BEGIN VOLCANO_BEGIN namespace UI {
#define VOLCANO_UI_END } VOLCANO_END

#endif // VOLCANO_UI_COMMON_HPP
