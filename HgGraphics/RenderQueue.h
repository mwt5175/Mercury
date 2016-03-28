/*****************************************************

	RenderQueue.h
		- Render Queue

*****************************************************/

#ifndef RENDER_QUEUE_H_INCLUDED
#define RENDER_QUEUE_H_INCLUDED

#include <vector>
#include <algorithm>
#include <hgkernel/kernel.h>
#include "texture.h"

namespace hgg {

#define HG_RENDER_QUEUE_ELEMENT_MAX_TEXTURE 32

	//
	// all objects that can be called from the renderer
	// must derive from this
	//

HGAPI class HgRenderQueueClient {

public:

	virtual HGVOID HGCALL Render (HGUINT pass) = 0;
};

	//
	// base render queue element is just an key mapped to a client.
	// we do not care about the contents of this key here
	//

HGAPI class HgRenderQueueElement {

public:

	HgRenderQueueClient* client;

	HGUINT64 key;

	HG_INLINE HGBOOL operator < (HgRenderQueueElement& in);
};

HG_INLINE HGBOOL HgRenderQueueElement::operator < (HgRenderQueueElement& in) {

	return in.key < key;
}

	//
	// defines the base of the render queue.
	//

HGAPI class HgRenderQueueBase {

protected:

	typedef std::vector<HgRenderQueueElement>           HgRenderQueueMap;
	typedef std::vector<HgRenderQueueElement>::iterator HgRenderQueueMapIter;

	HgRenderQueueMap queue;

	HG_INLINE HGVOID sort ();

	HG_INLINE HGVOID clear ();

	HG_INLINE HGUINT size ();
};

HG_INLINE HGVOID HgRenderQueueBase::sort () {

	std::sort(queue.begin(), queue.end());
}

HG_INLINE HGVOID HgRenderQueueBase::clear () {

	queue.clear();
}

HG_INLINE HGUINT HgRenderQueueBase::size () {

	return queue.size();
}

	//
	// defines a render state.
	//

HGAPI struct HgRenderState {

	HGFLOAT distance;

	HGUINT32 material;

	HGUINT8 pass;
};

	//
	// render queue. This defines the format of the key
	// which is used by sort to prioritize the render calls.
	//

HGAPI class HgRenderQueue : public HgRenderQueueBase {

public:

	HG_INLINE HGVOID add (HgRenderQueueClient& client, HgRenderState& state);

	HG_INLINE HGVOID render ();

};

	//
	// separation of opaque and normal,additive,subtractive translucent geometry
	//

HGAPI class HgRenderQueueGroup {

public:

	HgRenderQueue opaque;

	HgRenderQueue translucent;
};

}

#endif
