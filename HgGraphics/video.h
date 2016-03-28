/*****************************************************

	video.h
		Video Services

*****************************************************/

/*
	This component provides the graphics device interface
	that is linked with a graphics device driver library.
*/

#ifndef HG_VIDEO
#define HG_VIDEO

#ifdef QT
#include <tr1/memory>
#endif

#include <hgstd/base.h>

#include <hgmath/vector.h>
#include <hgmath/matrix.h>
#include <hgmath/rect.h>
#include <hgmath/orientation.h>
#include <hgmath/frustum.h>

#include "hwVertexBuffer.h"
#include "hwIndexBuffer.h"
#include "color.h"
#include "texture.h"
#include "shader.h"
#include "renderqueue.h"

namespace hgg {

enum HG_TRANSFORM {
	HG_T_MODELVIEW,
	HG_T_PROJECTION
};

enum HG_DEPTH_FUNCTION {
	HG_DF_NEVER,
	HG_DF_LESS,
	HG_DF_EQUAL,
	HG_DF_LEQUAL,
	HG_DF_GREATER,
	HG_DF_NOTEQUAL,
	HG_DF_GEQUAL,
	HG_DF_ALWAYS
};

enum HG_CULL_MODE {
	HG_CM_CW,
	HG_CM_CCW
};

enum HG_BLEND_FUNCTION {
	HG_BF_ZERO,
	HG_BF_ONE,
	HG_BF_SRC_COLOR,
	HG_BF_INV_SRC_COLOR,
	HG_BF_DST_COLOR,
	HG_BF_INV_DST_COLOR,
	HG_BF_SRC_ALPHA,
	HG_BF_INV_SRC_ALPHA,
	HG_BF_DST_ALPHA,
	HG_BF_INV_DST_ALPHA
};

typedef std::tr1::shared_ptr<HgVertexHardwareBuffer> HgVertexBufferSharedPtr;
typedef std::tr1::shared_ptr<HgIndexHardwareBuffer>  HgIndexBufferSharedPtr;
typedef std::tr1::shared_ptr<HgShaderProgram>        HgShaderProgramSharedPtr;
typedef std::tr1::shared_ptr<HgTexture>              HgTextureSharedPtr;

class HgVideoDriverBase {

protected:

	hgm::HgMatrix                world;

	HGBOOL                       worldDirty;

	hgm::HgMatrix                view;

	HGBOOL                       viewDirty;

	hgm::HgMatrix                projection;

	HGBOOL                       projectionDirty;

public:

	HG_INLINE hgm::HgMatrix&      getWorldTransform ();

};


HG_INLINE hgm::HgMatrix& HgVideoDriverBase::getWorldTransform () {
	return world;
}



class HgVideoDriver : public HgVideoDriverBase {

public:

	virtual	HGBOOL                   start           () = 0;

	virtual HGVOID                   startFrame      (hgg::HgColor back, HGBOOL clear = true, HGBOOL clearDepth = true) = 0;

	virtual HGVOID                   endFrame         () = 0;

	virtual HGVOID                   clearZBuffer     () = 0;

	virtual HGVOID                   setViewport      (hgm::HgRect& view) = 0;

	virtual hgm::HgRect&             getViewport      () = 0;

	virtual HGVOID                   setDepthTest     (HGBOOL enable) = 0;

	virtual HGVOID                   setDepthFunction (HG_DEPTH_FUNCTION function) = 0;

	virtual HGVOID                   setDepthWrite    (HGBOOL enable) = 0;

	virtual HGVOID                   setCulling       (HGBOOL enable) = 0;

	virtual HGVOID                   setCullMode      (HG_CULL_MODE mode) = 0;

	virtual HGVOID                   setBlend         (HGBOOL enable) = 0;

	virtual HGVOID                   setBlendFunction (HG_BLEND_FUNCTION function) = 0;

	virtual HgIndexBufferSharedPtr   newIndexBuffer   (HgIndexType type, size_t numIndices) = 0;

	virtual HgVertexBufferSharedPtr  newVertexBuffer  (size_t count, size_t size) = 0;

	virtual HgTextureSharedPtr       newTexture       () = 0;

	virtual HgShaderProgramSharedPtr newShaderProgram () = 0;
};

} // namespace hgg

#endif
