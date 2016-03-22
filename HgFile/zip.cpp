/*****************************************************

	archiveZip.cpp
		ZIP Archive HgFile reader

*****************************************************/

#include <iostream>
#include "zip.h"

extern "C" {
#include <minizip/unzip.h>
}

namespace hg {

//! opens zip HgFile for reading
bool archiveZip::open (std::string HgFilename) {

	zipFile = unzOpen (HgFilename.c_str ());

	if (!zipFile) {

		HgFileOpened = false;
		return false;
	}

	HgFileOpened = true;
	isClosed = false;
	fname = HgFilename;
	return true;
}

//! close HgFile
void archiveZip::close () {

	if (isClosed)
		return;

	if (zipFile) {

		if (HgFileOpened)
			unzCloseCurrentFile (zipFile);

		unzClose (zipFile);

		HgFileOpened=false;
		isClosed = true;
		zipFile = 0;
	}	
}

//! finds a HgFile in the zip HgFile
bool archiveZip::findFile (std::string HgFilename) {

	int res=0;

	if (zipFile)
		if (unzLocateFile (zipFile, HgFilename.c_str (), 0) == UNZ_OK )
			return true;

	return false;
}


//! finds and opens a HgFile
bool archiveZip::openFile (std::string HgFilename) {

	if (findFile (HgFilename) )
		if (unzOpenCurrentFile ( zipFile ) == UNZ_OK ) {

			unz_file_info	info;

			int res = 0;
			if ( res = unzGetCurrentFileInfo ( zipFile, &info, 0, 0, 0, 0, 0, 0 ) != UNZ_OK ) {

				unzCloseCurrentFile (zipFile);
				HgFileOpened =  false;
				return false;
			}

			std::cout << "[archiveZip] found " << HgFilename << " in " <<
				this->getZipFileName() << std::endl;

			uncompressedSize = info.uncompressed_size;
			HgFileOpened = true;
			return true;
		}

	HgFileOpened = false;
	return false;
}

//! read an opened HgFile
bool archiveZip::read (char* buf, unsigned int length) {

	if ( length > uncompressedSize )
		return false;

	if (unzReadCurrentFile ( zipFile, buf, length ) != UNZ_OK )
		return false;

	return true;
}


}//namespace mercury
