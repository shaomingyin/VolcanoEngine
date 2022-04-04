//
//
#include <Volcano/Editor/ToolChain.hpp>

VOLCANO_EDITOR_BEGIN

ToolChain::ToolChain(void):
    ProjectExplorer::ToolChain(ID)
{
}

ToolChain::~ToolChain(void)
{
}

Utils::LanguageExtensions ToolChain::languageExtensions(const QStringList &cxxflags) const
{
    return Utils::LanguageExtensions();
}

Utils::WarningFlags ToolChain::warningFlags(const QStringList &cflags) const
{
    return Utils::WarningFlags();
}

ToolChain::MacroInspectionRunner ToolChain::createMacroInspectionRunner() const
{
    return ToolChain::MacroInspectionRunner();
}

void ToolChain::addToEnvironment(Utils::Environment &env) const
{
}

Utils::FilePath ToolChain::makeCommand(const Utils::Environment &env) const
{
    return Utils::FilePath();
}

ToolChain::BuiltInHeaderPathsRunner ToolChain::createBuiltInHeaderPathsRunner(const Utils::Environment &env) const
{
    return ToolChain::BuiltInHeaderPathsRunner();
}

QList<Utils::OutputLineParser *> ToolChain::createOutputParsers(void) const
{
    return QList<Utils::OutputLineParser *>();
}

std::unique_ptr<ProjectExplorer::ToolChainConfigWidget> ToolChain::createConfigurationWidget(void)
{
    return nullptr;
}

VOLCANO_EDITOR_END
