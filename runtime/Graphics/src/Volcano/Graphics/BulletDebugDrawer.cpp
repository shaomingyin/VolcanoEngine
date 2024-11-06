//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/BulletDebugDrawer.h>

VOLCANO_GRAPHICS_BEGIN

BulletDebugDrawer::BulletDebugDrawer()
	: debug_mode_(DBG_NoDebug) {
}

void BulletDebugDrawer::init() {
}

void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
}

void BulletDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
}

void BulletDebugDrawer::reportErrorWarning(const char* text) {
	logWarning("Bullet Error: {}", text);
}

void BulletDebugDrawer::draw3dText(const btVector3& location, const char* textString) {
}

void BulletDebugDrawer::setDebugMode(int debug_mode) {
	debug_mode_ = debug_mode;
	switch (debug_mode_) {
	}
}

int BulletDebugDrawer::getDebugMode() const {
	return debug_mode_;
}

VOLCANO_GRAPHICS_END
