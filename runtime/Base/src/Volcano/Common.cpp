//
//
#include <Volcano/Common.h>

VOLCANO_BEGIN

static std::string app_organization;
static std::string app_name;

const std::string& appOrganization() {
    return app_organization;
}

const std::string& appName() {
    return app_name;
}

void setAppInfo(const std::string& organization, const std::string& name) {
    VOLCANO_ASSERT(app_organization.empty());
    VOLCANO_ASSERT(app_name.empty());
    app_organization = organization;
    app_name = name;
}

VOLCANO_END
