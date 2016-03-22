/*****************************************************

	HgFile.cpp
		System File

*****************************************************/

#include <fstream>
#include "file.h"

namespace hg {

bool HgFile::open (std::string FileName, bool append) {

	pkFile = fopen (FileName.c_str(), "rb");

	if (pkFile) {

		opened = true;
		HgFilename = FileName;
		return true;
	}

	return false;
}

void HgFile::close () {

	if (pkFile) {

		fclose (pkFile);
		pkFile = 0;
	}
}

bool HgFile::read (char* buf, unsigned int len) {

	if (buf)
		if (pkFile) {
			fread (buf, len, 1, pkFile);
			return true;
		}

	return false;
}

bool HgFile::write (char* buf, unsigned int len) {

	if (buf)
		if (pkFile) {
			fwrite (buf, len, 1, pkFile);
			return true;
		}

	return false;
}

void HgFile::seekg (unsigned int i) {

	if (pkFile)
		fseek (pkFile, i, SEEK_CUR);
}

void HgFile::seekp (unsigned int i) {

	if (pkFile)
		fseek (pkFile, i, SEEK_SET);
}

unsigned int HgFile::tellg () {

	if (pkFile)
		return ftell (pkFile);

	return 0;
}

unsigned int HgFile::tellp () {

	if (stream.is_open () )
		return stream.tellp ();

	return 0;
}

bool HgFile::eof () {

	int ret = feof (pkFile);
	return (!ret) ? false : true;
}

char HgFile::getc () {

	if (pkFile)
		return fgetc (pkFile);

	return 0;
}

void HgFile::rewind () {

	if (pkFile)
		::rewind (pkFile);
}

int HgFile::getSize () {

	int HgFileLen = 0;

	if (pkFile) {

		fseek(pkFile, 0, SEEK_END);
		HgFileLen = ftell ( pkFile );
		fseek(pkFile, 0, SEEK_SET);
	}

	return HgFileLen;
}

}//namespace mercury
