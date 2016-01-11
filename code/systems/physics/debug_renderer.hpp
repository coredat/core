#ifndef DEBUG_RENDERER_INCLUDED_2A10498F_02F2_403A_A882_D67983FA31D9
#define DEBUG_RENDERER_INCLUDED_2A10498F_02F2_403A_A882_D67983FA31D9


#include <LinearMath/btIDebugDraw.h>
#include <stdint.h>


namespace Physics {


/*!
  Renders debug information.
  This is slightly annoying of a class as it needs to integrate
  with other systems in order to render.
*/
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