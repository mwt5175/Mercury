/*****************************************************

	vidOgl.h
		OpenGL Video Driver API

*****************************************************/

#ifndef VIDEO_OGL_H_INCLUDED
#define VIDEO_OGL_H_INCLUDED

#include <hggraphics/video.h>

#include "shader.h"

#ifndef __gl_h_
#include "glee.h"
#endif

namespace hgogl {

/**
*	Video driver
*/
class HgVideoDriverOGL : public hgg::HgVideoDriver {

private:

	bool          gFog;

	bool		  wireframe;

	bool		  fullscreen;

	std::string   caption;

	void CreateGLMatrix( GLfloat glMatrix[16], const hgm::HgMatrix& m);

	void displayFormatInformation (std::string&) {}

	void stop ();

	void getDriverInfo ();

	bool genericStart ();

public:

	HgVideoDriverOGL();

	virtual ~HgVideoDriverOGL();

	virtual	HGBOOL                   start           ();

	virtual HGVOID                   startFrame      (hgg::HgColor back, HGBOOL clear = true, HGBOOL clearDepth = true);

	virtual HGVOID                   endFrame         ();

	virtual HGVOID                   clearZBuffer     ();

	virtual HGVOID                   setViewport      (hgm::HgRect& view);

	virtual hgm::HgRect&             getViewport      ();

	virtual HGVOID                   setDepthTest     (HGBOOL enable);

	virtual HGVOID                   setDepthFunction (hgg::HG_DEPTH_FUNCTION function);

	virtual HGVOID                   setDepthWrite    (HGBOOL enable);

	virtual HGVOID                   setCulling       (HGBOOL enable);

	virtual HGVOID                   setCullMode      (hgg::HG_CULL_MODE mode);

	virtual HGVOID                   setBlend         (HGBOOL enable);

	virtual HGVOID                   setBlendFunction (hgg::HG_BLEND_FUNCTION function);

	virtual hgg::HgIndexBufferSharedPtr   newIndexBuffer   (hgg::HgIndexType type, size_t numIndices);

	virtual hgg::HgVertexBufferSharedPtr  newVertexBuffer  (size_t count, size_t size);

	virtual hgg::HgTextureSharedPtr       newTexture       ();

	virtual hgg::HgShaderProgramSharedPtr newShaderProgram ();
};

}


#endif
