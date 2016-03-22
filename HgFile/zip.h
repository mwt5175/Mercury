/*****************************************************

	archiveZip.h
		ZIP Archive HgFile reader

*****************************************************/

#ifndef ARCHIVEZIP_H_INCLUDED
#define ARCHIVEZIP_H_INCLUDED

#include "file.h"

#include <hgstd/pack.h>

namespace hg {

/**
*	Local HgFile header
*/
class zipLocalFileHeader {

	unsigned int signiture;
	unsigned short extractVersion;
	unsigned short genBitFlag;
	unsigned short compressionMethod;
	unsigned short lastModifiedFileTime;
	unsigned short lastModifiedFileDate;
	unsigned int crc32;
	unsigned int compressionSize;
	unsigned int uncompressionSize;
	unsigned short HgFilenameLength;
	unsigned short extraFieldLength;
};

class zipDataDescriptor {

	unsigned int crc32;
	unsigned int compressedSize;
	unsigned int uncompressedSize;
};

class zipCentralDirectory {

	unsigned int HgFileHeaderSigniture;
	unsigned short versionMadeBy;
	unsigned short versionToExtract;
	unsigned short genBitFlag;
	unsigned short compressionMethod;
	unsigned short lastModifiedFileTime;
	unsigned short lastModifiedFileDate;
	unsigned int crc32;
	unsigned int compressionSize;
	unsigned int uncompressionSize;
	unsigned short HgFilenameLength;
	unsigned short extraFieldLength;
	unsigned short HgFileCommentLength;
	unsigned short diskNumberStart;
	unsigned short internalFileAttrib;
	unsigned int externalFileAttrib;
	unsigned int localHeaderOffset;
};

class zipCentralDirectoryEnd {

	unsigned int signiture;
	unsigned short diskNum;
	unsigned short diskNumCentralDirStart;
	unsigned short centralDirTotalEntriesDisk;
	unsigned short centralDirTotalEntries;
	unsigned int centralDirSize;
	unsigned int centralDirOffsetDisk;
	unsigned short zipFileCommentLength;
};

#include <hgstd/upack.h>

/**
*	Zip archive parser
*/
class archiveZip {

	void*        zipFile;
	unsigned int uncompressedSize;
	bool         HgFileOpened;
	bool         isClosed;
	std::string  fname;

public:

	archiveZip ()
		: zipFile (0), uncompressedSize (0), HgFileOpened (false), isClosed (false) {}

	virtual ~archiveZip () {
//		close (); //will cause memory leaks if ran from dtor. Must call manually.
	}

	// opens a zip File for reading
	virtual bool open (std::string HgFilename);

	// closes the zip File
	virtual void close ();

	// finds a File name in the zip File
	virtual bool findFile (std::string HgFilename);

	// finds and opens a File
	virtual bool openFile (std::string HgFilename);

	// reads currently opened File (length should be big enough for uncompressed HgFile)
	virtual bool read (char* buf, unsigned int length);

	// returns opened File uncompressed size
	virtual inline unsigned int getUncompressedSize () {return uncompressedSize;}

	// returns opened zip File name
	virtual inline std::string getZipFileName () {return fname;}
};

}//namespace mercury

#endif
