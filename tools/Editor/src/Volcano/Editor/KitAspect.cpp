//
//
#include <QVariant>

#include <utils/layoutbuilder.h>
#include <qtsupport/qtkitaspect.h>

#include <Volcano/Editor/KitAspect.h>

VOLCANO_EDITOR_BEGIN

KitAspect::KitAspect(Context& context, ProjectExplorer::Kit* kit, const ProjectExplorer::KitAspectFactory* factory)
    : ProjectExplorer::KitAspect(kit, factory)
    , context_(context) {
    setId(KIT_ASPECT);
    setAutoApply(false);
    setManagingPage(SDK_SETTINGS_PAGE);

    auto model = &context_.sdkManager();
    Q_ASSERT(model != nullptr);

    model->setDataAccessor([this](const Sdk& sdk, int column, int role) {
        QVariant ret;
        if (column == 0) {
            switch (role) {
            case Qt::DisplayRole:
                ret = QVariant("Version - " + sdk.version().toString());
                break;
            case ProjectExplorer::KitAspect::IdRole:
                ret = sdk.id().toSetting();
                break;
            case Qt::DecorationRole:
                break;
            default:
                break;
            }
        }
        return ret;
    });

    addListAspectSpec({ model,
        [model](const ProjectExplorer::Kit& kit) {
            auto id = Utils::Id::fromSetting(kit.value(SDK));
            if (id.isValid()) {
                return id.toSetting();
            }
            auto sdk = model->firstFitKit(kit);
            if (sdk != nullptr) {
                return sdk->id().toSetting();
            }
            return QVariant();
        },
        [model](ProjectExplorer::Kit& kit, const QVariant& id) {
            kit.setValue(SDK, id);
        },
        [model] {
            model->reset();
        }
    });
}

VOLCANO_EDITOR_END
