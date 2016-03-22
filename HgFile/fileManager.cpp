/*****************************************************

	HgFileManager.cpp
		File Manager

*****************************************************/

//#include <mercury.h>
#include "fileManager.h"
#include "virtualFile.h"
#include "zip.h"

#ifdef MERCURY_WINDOWS
//! WINDOWS SPECIFC
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include <iostream>

namespace hg {

/**
*	Closes all opened archive HgFiles
*/
HgFileManager::~HgFileManager () {

	closeAll ();
}

/**
*	Closes all opened archive HgFiles
*/
void HgFileManager::closeAll () {

	if (zipArchives.size () > 0)
		removeAllZipArchives ();

	if (pakArchives.size () > 0)
		removeAllPakArchives ();
}

/**
*	Opens a physical or virtual HgFile
*/
HgFile* HgFileManager::open (std::string HgFilename) {

	HgFile* pkFile = 0;

	//! search zip archives first
	for (unsigned int i = 0; i < zipArchives.size (); i++) {

		if (zipArchives[i]->openFile (HgFilename) ) {

			unsigned char* buf = new unsigned char [zipArchives[i]->getUncompressedSize () ];

			zipArchives[i]->read ((char*)buf, zipArchives[i]->getUncompressedSize() );

			if (buf) {

				pkFile = new HgVirtualFile (HgFilename, buf, zipArchives[i]->getUncompressedSize(), true);
				pkFile->read ((char*)buf, zipArchives[i]->getUncompressedSize());
				pkFile->setOpened (true);
			}

			return pkFile;
		}
	}

	//! search pak archives next
	for (unsigned int i = 0; i < pakArchives.size (); i++) {

		if (pakArchives[i]->openFile (HgFilename) ) {

			unsigned char* buf = new unsigned char [pakArchives[i]->getUncompressedSize () ];

			pakArchives[i]->read ((char*)buf, pakArchives[i]->getUncompressedSize() );

			if (buf) {

				pkFile = new HgVirtualFile (HgFilename, buf, pakArchives[i]->getUncompressedSize(), true);
				pkFile->read ((char*)buf, pakArchives[i]->getUncompressedSize());
				pkFile->setOpened (true);
			}

			return pkFile;
		}
	}

	//! not in archives, try loading from disk
	pkFile = new HgFile (HgFilename);

	if (!pkFile->isOpened()) {

		delete pkFile;
		pkFile=0;
		std::cout << "[FileManager] Unable to find " << HgFilename << std::endl;
		return 0;
	}

	std::cout << "[FileManager] " << HgFilename << " opened" << std::endl;
	return pkFile;
}

/**
*	Closes a HgFile
*/
void HgFileManager::close (HgFile* pkFile) {

	if (pkFile) {

		std::cout << "[FileManager] " << pkFile->getFilename() << " closed" << std::endl;
		pkFile->close ();
		delete pkFile;
		pkFile=0;
	}
}

/**
*	Creates a virtual HgFile
*/
HgFile* HgFileManager::openVirtual (std::string HgFilename, int l) {

	unsigned char* buf = new unsigned char [l];
	HgFile* pkFile = new HgVirtualFile (HgFilename, buf, l, true);
	return pkFile;
}

/**
*	Creates a physical directory
*/
bool HgFileManager::createDirectory (std::string path) {

#ifdef MERCURY_WINDOWS
//! WINDOWS SPECIFIC

	if (CreateDirectory (path.c_str(), 0) )
		return true;

#endif

	return false;
}

/**
*	Removes a physical directory
*/
bool HgFileManager::removeDirectory (std::string path) {

#ifdef MERCURY_WINDOWS
//! WINDOWS SPECIFIC

	if (RemoveDirectory (path.c_str () ) )
		return true;

#endif

	return false;
}

/**
*	Sets the current physical directory
*/
bool HgFileManager::setDirectory (std::string path) {

#ifdef MERCURY_WINDOWS
//! WINDOWS SPECIFIC

	if ( SetCurrentDirectory (path.c_str () ) ) {

		currentDirectory = path;
		return true;
	}

#endif

	return false;
}

/**
*	Return max path length
*/
int HgFileManager::getMaxPathLength () {

#ifdef MERCURY_WINDOWS
//! WINDOWS SPECIFIC

	return MAX_PATH;

#endif

	return 0;
}

/**
*	Get the HgFilename from a path
*/
std::string HgFileManager::HgFileFromPath (std::string path) {

	size_t i = path.find_last_of ("/\\");
	return std::string (path.substr (i+1) );
}

/**
*	Adds a zip archive
*/
bool HgFileManager::addZipArchive (std::string path) {

	archiveZip* archive = new archiveZip;

	if (! archive)
		return false;

	if (! archive->open (path) ) {

		delete archive;
		return false;
	}

	zipArchives.push_back (archive);
	return true;
}

/**
*	Removes a zip archive
*/
bool HgFileManager::removeZipArchive (std::string path) {

	std::vector <archiveZip*>::iterator it = zipArchives.begin ();

	for (; it != zipArchives.end (); it++ ) {

		if (*it) {

			std::string fname;
			fname = (*it)->getZipFileName ();

			if (fname == path) {

				delete (*it);
				zipArchives.erase (it);
				return true;
			}
		}
	}

	return false;
}

/**
*	Removes all zip archives
*/
void HgFileManager::removeAllZipArchives () {

	std::vector <archiveZip*>::iterator it = zipArchives.begin ();

	for (; it != zipArchives.end (); it++ ) {

		if (*it) {

			(*it)->close ();
			delete (*it);
		}
	}

	zipArchives.clear ();
}

/**
*	Adds a pak archive
*/
bool HgFileManager::addPakArchive (std::string path) {

	archiveZip* archive = new archiveZip;

	if (! archive)
		return false;

	if (! archive->open (path) ) {

		delete archive;
		return false;
	}

	pakArchives.push_back (archive);
	return true;
}

/**
*	Removes a pak archive
*/
bool HgFileManager::removePakArchive (std::string path) {

	std::vector <archiveZip*>::iterator it = pakArchives.begin ();

	for (; it != pakArchives.end (); it++ ) {

		if (*it) {

			std::string fname;
			fname = (*it)->getZipFileName ();

			if (fname == path) {

				delete (*it);
				pakArchives.erase (it);
				return true;
			}
		}
	}

	return false;
}

/**
*	Removes all pak archives
*/
void HgFileManager::removeAllPakArchives () {

	std::vector <archiveZip*>::iterator it = pakArchives.begin ();

	for (; it != pakArchives.end (); it++ ) {

		if (*it) {

			(*it)->close ();
			delete (*it);
		}
	}

	pakArchives.clear ();
}

}//namespace mercury
