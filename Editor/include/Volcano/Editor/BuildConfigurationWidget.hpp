//
//
#ifndef VOLCANO_EDITOR_BUILDCONFIGURATIONWIDGET_HPP
#define VOLCANO_EDITOR_BUILDCONFIGURATIONWIDGET_HPP

#include <projectexplorer/namedwidget.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class BuildConfiguration;

class BuildConfigurationWidget: public ProjectExplorer::NamedWidget {
public:
    BuildConfigurationWidget(BuildConfiguration *bc);

private:
    BuildConfiguration *m_buildConfiguration;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDCONFIGURATIONWIDGET_HPP
