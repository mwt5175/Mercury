/*****************************************************

	HgVirtualFile.cpp
		Virtual File

*****************************************************/

#include "virtualFile.h"

namespace hg {

bool HgVirtualFile::open (std::string fname, unsigned char* HgFile, unsigned int length) {

	HgFilename = fname;
	size = length;
	pkFile = HgFile;
	curGet = 0;
	curPut = 0;
	isEof = false;
	return true;
}

void HgVirtualFile::close () {

	if (release)
		if (pkFile) {
			delete [] pkFile;
			pkFile = 0;
		}
}

bool HgVirtualFile::eof () {

	return isEof;
}

bool HgVirtualFile::read (char* buf, unsigned int len) {

	if (isEof)
		return false;

	if (curGet == size) {

		isEof = true;
		return false;
	}

	isEof = false;

	if (curGet + len > size)
		len = curGet - len;

	memcpy (buf, pkFile+curGet, len);
	curGet += len;
	return true;
}

bool HgVirtualFile::write (char* buf, unsigned int len) {

	if (isEof)
		return false;

	if (curPut == size) {

		isEof = true;
		return false;
	}

	isEof = false;

	if (curPut + len > size)
		len = curGet - len;

	memcpy (pkFile+curPut, buf, len);
	curPut += len;
	return true;
}

void HgVirtualFile::seekg (unsigned int newGet) {

	if (newGet >= size) {

		isEof = true;
		return;
	}

	isEof = false;
	curGet = newGet;
}

void HgVirtualFile::seekp (unsigned int newPut) {

	if (newPut >= size) {

		isEof = true;
		return;
	}

	isEof = false;
	curPut = newPut;
}

unsigned int HgVirtualFile::tellg () {

	return curGet;
}

unsigned int HgVirtualFile::tellp () {

	return curPut;
}

char HgVirtualFile::getc () {

	char c=0;
	read (&c, 1);
	return c;
}

}//namespace mercury
