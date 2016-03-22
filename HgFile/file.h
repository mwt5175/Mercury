/*****************************************************

	HgFile.h
		System File

*****************************************************/

#include <string>
#include <fstream>

#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

namespace hg {

/**
*	File type information
*/
enum HgFileType {
	HG_FILE_PHYSICAL,
	HG_FILE_VIRTUAL
};

/**
*	Used to read/write HgFiles on disk
*/
class HgFile {

protected:

	std::string  HgFilename;
	bool         opened;
	unsigned int size;
	std::fstream stream;
	FILE*        pkFile;

public:

	HgFile ()
		: HgFilename (""), opened (false), size (0) {}

	HgFile (std::string HgFilename, bool append=false)
		: opened (false) {

		if ( open (HgFilename, append ) )
			opened = true;
	}

	virtual ~HgFile () {

		if (opened)
			close ();

		opened = false;
	}

	//! setters and getters
	inline HgFileType getType () {return HG_FILE_PHYSICAL;}
	inline bool isOpened () {return opened;}
	inline void setOpened (bool o) {opened=o;}
	inline std::string getFilename () {return HgFilename;}
	inline void setFilename (std::string f) {HgFilename=f;}
	inline FILE *getCFilePtr() {return pkFile;};

	// get File extension
	inline std::string getFileExtension () {
		std::string ext;
		std::string::size_type idx = HgFilename.rfind ('.');
		if (idx != std::string::npos)
			ext = HgFilename.substr (idx+1);

		return ext;
	}

	virtual bool open (std::string HgFilename, bool append=false);
	virtual void close ();
	virtual bool read (char* buf, unsigned int len);
	virtual bool write (char* buf, unsigned int len);
	virtual void seekg (unsigned int);
	virtual void seekp (unsigned int);
	virtual unsigned int tellg ();
	virtual unsigned int tellp ();
	virtual bool eof ();
	virtual char getc ();
	virtual void rewind ();
	virtual int getSize ();
};

}//mercury

#endif
