/*****************************************************

	HgVirtualFile.h
		Virtual File

*****************************************************/

#ifndef VIRTUALFILE_H_INCLUDED
#define VIRTUALFILE_H_INCLUDED

#include <string>
#include "file.h"

namespace hg {

/**
*	A virtual HgFile is a HgFile in memory
*/
class HgVirtualFile : public HgFile {

	unsigned char* pkFile;
	unsigned int   curPut;
	unsigned int   curGet;
	bool           isEof;
	bool           release;

public:

	HgVirtualFile (std::string fname, unsigned char* HgFile, unsigned int length, bool releaseMem=false)
		: pkFile (0), curPut (0), curGet (0), isEof (false), release (releaseMem) {

		open (fname, HgFile, length);
	}

	virtual ~HgVirtualFile () {

		setOpened (false);
		close ();
	}

	virtual inline HgFileType getType () {return HG_FILE_VIRTUAL;}
	virtual inline unsigned char* getFileData () {return pkFile;}
	virtual void close ();
	virtual bool open (std::string HgFilename, unsigned char* HgFile, unsigned int length);
	virtual bool read (char* buf, unsigned int len);
	virtual bool write (char* buf, unsigned int len);
	virtual bool eof ();
	virtual void seekg (unsigned int);
	virtual void seekp (unsigned int);
	virtual unsigned int tellg ();
	virtual unsigned int tellp ();
	virtual char getc ();
	virtual int getSize () {return 0;}
};

}//mercury

#endif
