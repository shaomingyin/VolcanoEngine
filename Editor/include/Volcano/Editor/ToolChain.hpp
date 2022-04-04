//
//
#ifndef VOLCANO_EDITOR_TOOLCHAIN_HPP
#define VOLCANO_EDITOR_TOOLCHAIN_HPP

#include <projectexplorer/toolchain.h>
#include <projectexplorer/toolchainconfigwidget.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class ToolChain: public ProjectExplorer::ToolChain {
public:
    static constexpr char ID[] = "VolcanoEditor.ToolChain";

public:
    ToolChain(void);
    ~ToolChain(void) override;

public:
    Utils::LanguageExtensions languageExtensions(const QStringList &cxxflags) const override;
    Utils::WarningFlags warningFlags(const QStringList &cflags) const override;
    MacroInspectionRunner createMacroInspectionRunner(void) const override;
    void addToEnvironment(Utils::Environment &env) const override;
    Utils::FilePath makeCommand(const Utils::Environment &env) const override;
    BuiltInHeaderPathsRunner createBuiltInHeaderPathsRunner(const Utils::Environment &env) const override;
    QList<Utils::OutputLineParser *> createOutputParsers(void) const override;
    std::unique_ptr<ProjectExplorer::ToolChainConfigWidget> createConfigurationWidget(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_TOOLCHAIN_HPP
