/*****************************************************

	hwBuffer.h
		Hardware Buffer

*****************************************************/

#ifndef HWBUFFER_H_INCLUDED
#define HWBUFFER_H_INCLUDED

namespace hgg {

/**
*	Hardware buffer usage flags
*/
enum HgBufferUsage {
	HG_BUFFER_STATIC_READ_ONLY,
	HG_BUFFER_STATIC_WRITE_ONLY,
	HG_BUFFER_DYNAMIC_READ_ONLY,
	HG_BUFFER_DYNAMIC_WRITE_ONLY,
	HG_BUFFER_STREAM_READ,
	HG_BUFFER_STREAM_WRITE
};

/**
*	Hardware buffer
*/
class HgHardwareBuffer {

protected:

	bool           locked;
	unsigned int   offset;
	unsigned int   size;
	bool           systemMemory;
	HgBufferUsage  usage;

	virtual void* _lock   (unsigned int offset, unsigned int size) = 0;
	virtual void  _unlock () = 0;

public:

	HgHardwareBuffer ()
		: locked(false), offset(0), size(0), systemMemory (false) {}

	void* lock (unsigned int offset, unsigned int size) {
		if (locked)
			return 0;
		void* ret = _lock (offset, size);
		if (ret)
			locked = true;
		return ret;
	}

	void unlock () {
		if (!locked)
			return;
		_unlock ();
		locked = false;
	}

	virtual void read (void* data, unsigned int offset, unsigned int size) = 0;
	virtual void write (void* data, unsigned int offset, unsigned int size) = 0;

	inline bool isLocked ()                const {return locked;}
	inline bool isSystemMemory ()          const {return systemMemory;}
	inline HgBufferUsage getUsage () const {return usage;}
};

} // namespace hgg

#endif
