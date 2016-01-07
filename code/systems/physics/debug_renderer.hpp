#ifndef DEBUG_RENDERER_INCLUDED_
#define DEBUG_RENDERER_INCLUDED_


#include <LinearMath/btIDebugDraw.h>
#include <stdint.h>


namespace Physics {


class Debug_renderer : public btIDebugDraw
{
public:

  explicit      Debug_renderer();
  virtual       ~Debug_renderer();
	
	void          drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;
	void          drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override;
	
	void          reportErrorWarning(const char* warningString) override;
	void          draw3dText(const btVector3 &location, const char *textString) override;
	void          setDebugMode(int debugMode) override;
  int           getDebugMode() const override;
	
private:

	uint32_t      m_debug_mode;
	
}; // class



} // ns


#endif // inc guard