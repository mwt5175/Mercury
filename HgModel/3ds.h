/*****************************************************

	3ds.h
		3DS File

*****************************************************/

#ifndef H_INCLUDED_3DS
#define H_INCLUDED_3DS

#include <fstream>
#include <map>
#include <vector>

namespace mercury {

/**
*	3ds chunk identifiers
*/

//! primary chunk
#define MAIN3DS      0x4D4D

//! main chunks
#define EDIT3DS      0x3D3D     //start of editor config
#define KEYF3DS      0xB000     //start of the keyframer config

//! sub defines of EDIT3DS
#define EDIT_MATERIAL 0xAFFF
#define EDIT_CONFIG1  0x0100
#define EDIT_CONFIG2  0x3E3D
#define EDIT_VIEW_P1  0x7012
#define EDIT_VIEW_P2  0x7011
#define EDIT_VIEW_P3  0x7020
#define EDIT_VIEW1    0x7001
#define EDIT_BACKGR   0x1200
#define EDIT_AMBIENT  0x2100
#define EDIT_OBJECT   0x4000

#define EDIT_UNKNW01  0x1100
#define EDIT_UNKNW02  0x1201
#define EDIT_UNKNW03  0x1300
#define EDIT_UNKNW04  0x1400
#define EDIT_UNKNW05  0x1420
#define EDIT_UNKNW06  0x1450
#define EDIT_UNKNW07  0x1500
#define EDIT_UNKNW08  0x2200
#define EDIT_UNKNW09  0x2201
#define EDIT_UNKNW10  0x2210
#define EDIT_UNKNW11  0x2300
#define EDIT_UNKNW12  0x2302 
#define EDIT_UNKNW13  0x3000
#define EDIT_UNKNW14  0xAFFF

//>------ sub defines of EDIT_OBJECT
#define OBJ_TRIMESH   0x4100
#define OBJ_LIGHT     0x4600
#define OBJ_CAMERA    0x4700

#define OBJ_UNKNWN01  0x4010
#define OBJ_UNKNWN02  0x4012 //>>---- Could be shadow

//>------ sub defines of OBJ_CAMERA
#define CAM_UNKNWN01  0x4710 
#define CAM_UNKNWN02  0x4720 

//>------ sub defines of OBJ_LIGHT
#define LIT_OFF       0x4620
#define LIT_SPOT      0x4610
#define LIT_UNKNWN01  0x465A

//>------ sub defines of OBJ_TRIMESH
#define TRI_VERTEXL   0x4110
#define TRI_FACEL2    0x4111 
#define TRI_FACEL1    0x4120
#define TRI_TEXCOORD  0x4140	// DAS 11-26-04
#define TRI_SMOOTH    0x4150
#define TRI_LOCAL     0x4160
#define TRI_VISIBLE   0x4165

//>>------ sub defs of KEYF3DS
#define KEYF_UNKNWN01 0xB009
#define KEYF_UNKNWN02 0xB00A
#define KEYF_FRAMES   0xB008
#define KEYF_OBJDES   0xB002

//>>------  these define the different color chunk types
#define COL_RGB  0x0010
#define COL_TRU  0x0011
#define COL_UNK  0x0013

//>>------ defines for viewport chunks
#define TOP           0x0001
#define BOTTOM        0x0002
#define LEFT          0x0003
#define RIGHT         0x0004
#define FRONT         0x0005
#define BACK          0x0006
#define USER          0x0007
#define CAMERA        0x0008 // 0xFFFF is the actual code read from file
#define LIGHT         0x0009
#define DISABLED      0x0010
#define BOGUS         0x0011

/**
*	3ds chunk
*/
class chunk3ds {

	int chunkId;
	int begin, end;
	int chunkSetEnd;
	std::ifstream& kFile;

public:

	chunk3ds(std::ifstream& file, int cset);

	chunk3ds (const chunk3ds& chunk)
		: chunkSetEnd (chunk.chunkSetEnd),
		chunkId (chunk.chunkId),
		begin (chunk.begin),
		end (chunk.end),
		kFile (chunk.kFile)
	{
	}

	virtual ~chunk3ds ();

	operator bool ();
	void operator = ( chunk3ds& );
	inline int id () { return chunkId; }

	chunk3ds child ();
	chunk3ds sibling ();

	//! Reads data elements from chunk
	short Short ();
	int Int ();
	float Float ();
	std::string str ();
};

/**
*	light vertex
*/
class LightVertex3ds {

public:

	float x;
	float y;
	float z;
};

/**
*	3ds light object
*/
class Light3ds {

	LightVertex3ds pos;
	LightVertex3ds targ;
	bool spotLight;

public:

	Light3ds ()
		: spotLight(false) {
	}

	Light3ds (chunk3ds c)
		: spotLight (false) {

		initialize (c);
	}

	inline bool spotlight () {return spotLight;}
	inline LightVertex3ds& position () {return pos;}
	inline LightVertex3ds& target () {return targ;}
	void initialize (chunk3ds);
};

/**
*	3ds material object
*/
class material3dsObj {

	std::string text;
	float shiny;  // percent
	float transp; // percent

public:

	material3dsObj ()
		: shiny(0.0f),
		transp(0.0f) {
	}

	material3dsObj (std::string te, float s, float t)
		: text(te),
		shiny(s),
		transp(t) {
	}

	material3dsObj (const material3dsObj& s) {

		operator = (s);
	}

	inline float shininess () {return shiny;}
	inline float transparency () {return transp;}
	inline std::string texture () {return text;}

	void operator = ( material3dsObj );
};

/**
*	3ds materials
*/
class material3ds {

	//! list of materials by name
	std::map <std::string, material3dsObj> materials;
public:

	std::string map ( chunk3ds c );
	void initialize ( chunk3ds );
};

/**
*	3ds mesh vertex
*/
class meshVertex3ds {

public:
	float x;
	float y;
	float z;
};

/**
*	3ds texture coord
*/
class meshTexCoord3ds {

public:
	float u;
	float v;
};

/**
*	3ds face
*/
class meshFace3ds {

public:
	int a;
	int b;
	int c;
	int flag;
};

/**
*	3ds matrix
*/
class meshMatrix3ds {

public:
	float m[4][4];
};

/**
*	3ds mesh object
*/
class mesh3dsObj {

	void readLocalCoordSystem (chunk3ds);
	void readVertices (chunk3ds);
	void readTextCoords (chunk3ds);
	void readFaces (chunk3ds);
	void readFacesMaterials (chunk3ds);

public:

	mesh3dsObj (chunk3ds c) {

		initialize (c);
	}

	mesh3dsObj (const mesh3dsObj& c) {

		operator = (c);
	}

	meshMatrix3ds mat;
	std::vector <meshVertex3ds > vertices;
	std::vector <meshTexCoord3ds> textcoords;
	std::vector <meshFace3ds> faces;
	std::map <std::string, std::vector<int> > materialFaces;

	void operator = ( mesh3dsObj );
	void initialize ( chunk3ds );

};

/**
*	3ds model
*/
class model3ds {

	void parseEditor (chunk3ds);
	void parseEditObject (chunk3ds);

public:

	void load (std::string file);

};

}

#endif
