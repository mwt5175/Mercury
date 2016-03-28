/*****************************************************

	indexBuffer.h
		Index buffer

*****************************************************/

#ifndef VERTEXBUFFER_H_INCLUDED
#define VERTEXBUFFER_H_INCLUDED

#include <vector>
#include <hgstd/base.h>
#include <hggraphics/hwbuffer.h>

namespace hgg {

enum HgPrimitiveType {
	HG_POINTS,
	HG_LINE_STRIP,
	HG_LINE_LOOP,
	HG_LINES,
	HG_TRIANGLE_STRIP,
	HG_TRIANGLE_FAN,
	HG_TRIANGLES,
	HG_QUAD_STRIP,
	HG_QUADS,
	HG_POLYGON
};

enum HgVertexSemantic {
	HG_VERTEX_POSITION = 0,
	HG_VERTEX_COLOR,
	HG_VERTEX_NORMAL,
	HG_VERTEX_TEXTURE0,
	HG_VERTEX_TEXTURE1,
	HG_VERTEX_MATERIAL,
	HG_VERTEX_TANGENT,
	HG_VERTEX_BLEND_INDEX,
	HG_VERTEX_BLEND_WEIGHT
};

class HgVertexHardwareBuffer : public HgHardwareBuffer {

protected:

	size_t vertexCount;
	size_t vertexSize;

public:

	HgVertexHardwareBuffer (size_t count, size_t size)
		: vertexCount(count), vertexSize(0), HgHardwareBuffer() {
	
		HG_UNREFERENCED (size);
	}
	virtual ~HgVertexHardwareBuffer() {}
	inline size_t getVertexCount () const {return vertexCount;}
	inline size_t getVertexSize () const {return vertexSize;}
};

} // namespace hgg

#endif
