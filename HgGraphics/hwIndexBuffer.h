/*****************************************************

	indexBuffer.h
		Index buffer

*****************************************************/

#ifndef INDEXBUFFER_H_INCLUDED
#define INDEXBUFFER_H_INCLUDED

#include <vector>
#include <hgstd/base.h>
#include <hggraphics/hwBuffer.h>

namespace hgg {

enum HgIndexType {
	HG_INDEX_16BIT,
	HG_INDEX_32BIT
};

class HgIndexHardwareBuffer : public HgHardwareBuffer {

	HgIndexType type;
	size_t numIndices;

public:

	HgIndexHardwareBuffer (HgIndexType type, size_t numIndices) {
		HG_UNREFERENCED (numIndices);
		HG_UNREFERENCED (type);
	}
	virtual ~HgIndexHardwareBuffer () {}
	inline HgIndexType getType () const {return type;}
	inline size_t getIndexCount () const {return numIndices;}
};

} // namespace hgg

#endif
