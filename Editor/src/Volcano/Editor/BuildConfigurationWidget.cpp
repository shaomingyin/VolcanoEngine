//
//
#include <Volcano/Editor/BuildConfigurationWidget.hpp>

VOLCANO_EDITOR_BEGIN

BuildConfigurationWidget::BuildConfigurationWidget(BuildConfiguration *bc):
    ProjectExplorer::NamedWidget("Volcano"),
    m_buildConfiguration(bc)
{
    QTC_CHECK(bc);

    // TODO
}

VOLCANO_EDITOR_END
