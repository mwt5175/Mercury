/*****************************************************

	HgFileManager.h
		File Manager

*****************************************************/

#ifndef FILEMANAGER_H_INCLUDED
#define FILEMANAGER_H_INCLUDED

#include <vector>
#include "file.h"
#include "zip.h"

namespace hg {

/**
*	This creates a central location for all HgFile access: real, virtual, and archives
*/
class HgFileManager {

	std::string	currentDirectory;

	std::vector <archiveZip*>	zipArchives;

	std::vector <archiveZip*>	pakArchives;

public:

	virtual ~HgFileManager ();

	//! closes all opened archives
	virtual void closeAll ();

	//! opens a physical or virtual HgFile
	virtual HgFile* open (std::string HgFilename);

	//! closes a HgFile
	virtual void close (HgFile*);

	//! returns current directory path
	inline std::string& getCurrentDirectory () {return currentDirectory;}

	//! adds a ZIP HgFile archive
	virtual bool addZipArchive (std::string path);

	//! removes a ZIP HgFile archive
	virtual bool removeZipArchive (std::string path);

	//! removes all ZIP HgFile archives
	virtual void removeAllZipArchives ();

	//! adds a PAK HgFile archive
	virtual bool addPakArchive (std::string path);

	//! removes a ZIP HgFile archive
	virtual bool removePakArchive (std::string path);

	//! removes all ZIP HgFile archives
	virtual void removeAllPakArchives ();

	//! creates a subdirectory
	virtual bool createDirectory (std::string path);

	//! removes a directory
	virtual bool removeDirectory (std::string path);

	//! set the current subdirectory
	virtual bool setDirectory (std::string path);

	//! creates and opens a HgFile in memory (virtual HgFile)
	virtual HgFile* openVirtual (std::string HgFilename, int l);

	//! returns maximum path length allowed (This is OS-specific)
	virtual int getMaxPathLength ();

	//! returns HgFilename from a path
	virtual std::string HgFileFromPath (std::string path);
};

}//namespace mercury

#endif
