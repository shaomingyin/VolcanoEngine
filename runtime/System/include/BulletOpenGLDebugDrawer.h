//
//
#ifndef VOLCANO_GRAPHICS_BULLETDEBUGDRAWER_H
#define VOLCANO_GRAPHICS_BULLETDEBUGDRAWER_H

#include <bullet/LinearMath/btIDebugDraw.h>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class BulletDebugDrawer: public btIDebugDraw {
public:
	BulletDebugDrawer();
	~BulletDebugDrawer() override = default;

public:
	void init();
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
	void reportErrorWarning(const char* warningString) override;
	void draw3dText(const btVector3& location, const char* textString) override;
	void setDebugMode(int debugMode) override;
	int getDebugMode() const override;

private:
	int debug_mode_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BULLETDEBUGDRAWER_H
