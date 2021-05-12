//
//
#ifndef VOLCANO_EDITOR_COMMON_HPP
#define VOLCANO_EDITOR_COMMON_HPP

#include <memory>

#include <QtGlobal>
#include <QDebug>

#include <Volcano/Common.hpp>

#define VOLCANO_EDITOR_BEGIN VOLCANO_BEGIN namespace Editor {
#define VOLCANO_EDITOR_END } VOLCANO_END

VOLCANO_EDITOR_BEGIN

std::shared_ptr<spdlog::logger> logger(void);

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_COMMON_HPP
