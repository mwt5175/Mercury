/*****************************************************

	3ds.cpp
		3DS File

*****************************************************/

#include "3ds.h"

namespace mercury {

chunk3ds::chunk3ds(std::ifstream& file, int cset)

: kFile (file),
  chunkSetEnd (cset)
{
	//! get chunk beginning position
	begin = file.tellg ();

	//! read chunk id
	chunkId = 0;
	file.read ((char*)&chunkId, 2);

	//! get chunk end address
	file.read ((char*)&end, 4);
	end += begin;
}

chunk3ds::~chunk3ds () {

}

/**
*	Test if chunk is at end of parent chunk
*/
chunk3ds::operator bool () {

	return (begin < chunkSetEnd) && (begin >= 0);
}

chunk3ds chunk3ds::child () {

	//! return end of this parents chunk
	return chunk3ds (kFile, end);
}

chunk3ds chunk3ds::sibling () {

	//! pass this chunks parents end position
	kFile.seekg (end);
	return chunk3ds (kFile, chunkSetEnd);
}

short chunk3ds::Short () {

	short s=0;
	kFile.read ((char*)&s, 2);
	return s;
}

int chunk3ds::Int () {

	int s=0;
	kFile.read ((char*)&s, 4);
	return s;
}

float chunk3ds::Float () {

	float s=0;
	kFile.read ((char*)&s, 4);
	return s;
}

std::string chunk3ds::str () {

	std::string s;
	char c;

	do {

		kFile.read (&c, 1);
		s += c;

	}while (c != 0);

	return s;
}

void chunk3ds::operator = ( chunk3ds& c ) {

	//! set file position
	kFile.seekg (c.kFile.tellg () );

	//! assign members
	chunkSetEnd = c.chunkSetEnd;
	chunkId = c.chunkId;
	begin = c.begin;
	end=c.end;
}

/**
*	Initialized 3ds light object from chunk
*/
void Light3ds::initialize (chunk3ds c) {

	//! read data from chunk
	pos.x = c.Float ();
	pos.y = c.Float ();
	pos.z = c.Float ();

	chunk3ds d = c.child ();
	for (; ; d = d.sibling () ) {

		switch (d.id ()) {

			//! spot light?
			case LIT_SPOT: {

				//! read data from chunk
				spotLight=true;
				targ.x = c.Float();
				targ.y = c.Float();
				targ.z = c.Float ();
				break;
			}
		}
	}
}

void material3dsObj::operator = ( material3dsObj m ) {

	shiny = m.shiny;
	text = m.text;
	transp = m.transp;
}

/**
*	Obtains material name from chunk
*/
std::string material3ds::map ( chunk3ds c ) {

	chunk3ds d = c.child ();
	for (; ; d = d.sibling() ) {

		switch (d.id() ) {

			case 0xa300: {

				return d.str ();
			}
		}
	}

	return ("unknown");
}

/**
*	Initializes 3ds material from chunk
*/
void material3ds::initialize ( chunk3ds c ) {

	std::string name;
	std::string text;
	float shiny=0.0f;
	float transp=0.0f;

	chunk3ds d = c.child ();
	for (; ; d = d.sibling () ) {

		switch (d.id () ) {

			case 0xa000: {

				name = d.str ();
				break;
			}
 			case 0xa040: {

				shiny = 1.0f;
				break;
			}
  			case 0xa050: {

				transp = 1.0f;
				break;
			}
   			case 0xa200:
			case 0xa33a: {

				text = map (c);
				break;
			}
		}
	}

	material3dsObj mat ( text, shiny, transp );
//	materials [name] = mat;
}

/**
*	Reads the local coord system from chunk
*/
void mesh3dsObj::readLocalCoordSystem (chunk3ds c) {

	//! read matrix from chunk
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			mat.m[i][j] = c.Float ();

	//! last row must be 0,0,0,1 (idenitity)
	mat.m[0][3] = 0;
	mat.m[1][3] = 0;
	mat.m[2][3] = 0;
	mat.m[3][3] = 1;
}

/**
*	Reads vertices from chunk
*/
void mesh3dsObj::readVertices (chunk3ds c) {

	//! get number of vertixes from chunk
	int vertexCount = c.Short ();

	for (int i=0; i<vertexCount; i++) {

		//! read vertex from chunk
		meshVertex3ds v;
		v.x = c.Float ();
		v.y = c.Float ();
		v.z = c.Float ();

		//! store vertex in buffer
		vertices.push_back (v);
	}
}

/**
*	Reads texture coords from chunk
*/
void mesh3dsObj::readTextCoords (chunk3ds c) {

	//! get number of texture coords from chunk
	int texCoordCount = c.Short ();

	for (int i=0; i<texCoordCount; i++) {

		//! read texture coord from chunk
		meshTexCoord3ds coord;
		coord.u = c.Float ();
		coord.v = c.Float ();

		//! store it in buffer
		textcoords.push_back (coord);
	}
}

/**
*	Reads faces from chunk
*/
void mesh3dsObj::readFaces (chunk3ds c) {

	//! get number of faces from chunk
	int facesCount = c.Short ();

	for (int i=0; i<facesCount; i++) {

		//! read face data from chunk
		meshFace3ds face;
		face.a = c.Short ();
		face.b = c.Short ();
		face.c = c.Short ();
		face.flag = c.Short ();

		//! store in buffer
		faces.push_back (face);
	}

	chunk3ds d = c.child ();
	for (; ; d = c.sibling () ) {

		switch (d.id () ) {

			case 0x4130: {

				readFacesMaterials (c);
				break;
			}
		}
	}
}

/**
*
*/
void mesh3dsObj::readFacesMaterials (chunk3ds c) {

	//! get material name from chunk
	std::string matName = c.str ();

	//! get number of faces
	int facesCount = c.Short ();

	std::vector <int> facesApplied;

	//! get faces material is assigned to
	for (int i=0; i<facesCount; i++) {

		int f = c.Short ();
		facesApplied.push_back (f);
	}

//	materialFaces [matName] = facesApplied;
}

void mesh3dsObj::operator = ( mesh3dsObj c ) {

	mat = c.mat;
	vertices = c.vertices;
	textcoords = c.textcoords;
	faces = c.faces;
	materialFaces = c.materialFaces;
}

/**
*	Read mesh data from chunk
*/
void mesh3dsObj::initialize ( chunk3ds c ) {

	chunk3ds d = c.child ();
	for (; ; d = c.sibling () ) {

		switch (d.id () ) {

			case TRI_LOCAL: {

				readLocalCoordSystem (d);
				break;
			}
			case TRI_VERTEXL: {

				readVertices (d);
				break;
			}
			case TRI_TEXCOORD: {

				readTextCoords (d);
				break;
			}
			case TRI_FACEL1: {

				readFaces (c);
				break;
			}
		}
	}
}

int getFileSize (std::ifstream& file)
{
	int curr_pos = file.tellg();

	file.seekg(0);
	int beginning = file.tellg();
	file.seekg(0 , std::ios::end);
	int ending = file.tellg();

	file.seekg(curr_pos);

	return ending - beginning;
}

void model3ds::load (std::string file) {

	//! open file
	std::ifstream kFile( file.c_str() );

	//! get file size
	int fsize=getFileSize (kFile);

	//! find root chunk
	kFile.seekg (0);
	chunk3ds rootChunk (kFile, fsize);
/**
	chunk3ds c = rootCunk.child ();
	for (; ; c = rootChunk.sibling () ) {

		switch ( c.id () ) {

			case 0x0002: {

				//version info (parse data int)
				break;
			}
			case EDIT3DS: {

				parseEditor (c);
				break;
			}
		}
	}
**/
}

void model3ds::parseEditor (chunk3ds c) {

	chunk3ds d = c.child ();
	for (; ; d = d.sibling () ) {

		switch ( d.id () ) {

			case 0x3d3e: {

				//mesh version (parse data int)
				break;
			}
			case EDIT_MATERIAL: {

				//materials
				break;
			}
			case EDIT_OBJECT: {

				parseEditObject (d);
				break;
			}
		}
	}
}

void model3ds::parseEditObject (chunk3ds c) {

	chunk3ds d = c.child ();
	for (; ; d = d.sibling () ) {

//need to finish: parsing EDIT_OBJECT sub-defines

	}
}

}
