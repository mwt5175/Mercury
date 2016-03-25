/*****************************************************

	md2.h
		md2 codec

*****************************************************/

#ifndef MD2_H_INCLUDED
#define MD2_H_INCLUDED

#include <gfx/texture.h>
#include <gfx/meshCodec.h>
#include <gfx/meshMd2.h>
#include <mgr/file/file.h>
#include <mgr/scene/baseObject.h>

namespace mercury {

const float KEYFRAMES_PER_S = 8;

#ifdef _MSC_VER
#pragma pack (push, 1)
#endif

/**
*	Md2 vertex data
*/
typedef struct _Md2Vertex {

	union {

		struct {

			float x;
			float y;
			float z;
		};
		float v[3];
	};

}Md2Vertex, *PMd2_Vertex;

/**
*	Md2 texture coord
*/
typedef struct _Md2TexCoord {

	float s;
	float t;

}Md2TexCoord, *PMd2TexCoord;

/**
*	Md2 texture coord index
*/
typedef struct _Md2TexCoordIndex {

	short s;
	short t;

}Md2TexCoordIndex, *PMd2TexCoordIndex;

//RealST[i].s = (float)texCoord[i].s / header.skinwidth;
//RealST[i].t = (float)texCoord[i].t / header.skinheight;

/**
*	md2 frame point
*/
typedef struct _Md2FramePoint {

	unsigned char v[3]; //compressed vertex x,y,z coords
	unsigned char normalIndex; //index to a normal lighting vector

}Md2FramePoint, *PMd2FramePoint;

/**
*	md2 frame
*/
typedef struct _Md2Frame {

	float scale[3]; //scale vector
	float translate[3]; //translation vector
	char name[16]; //frame name
//	Md2FramePoint fp[1]; //first frame point

}Md2Frame, *PMd2Frame;

/**
*	md2 triangle
*/
typedef struct _Md2Mesh {

	unsigned short meshIndex[3]; //vertex indices
	unsigned short stIndex[3]; //texture coord indices

}Md2Mesh, *PMd2Mesh;

/**
*	md2 Face
*/
typedef struct _Md2Face {

	short p1, p2, p3;
	short uv1, uv2, uv3;

}Md2Face;

/**
*	soon to be generic 3d frame
*/
typedef struct _frame3d {

	Md2Vertex* vertexList;
	Md2Vertex* normals;

}frame3d;

/**
*	md2 header
*/
typedef struct _Md2Header {

	int magic; // should be IDP2
	int version; //should be 8

	int skinWidth; //width of skin texture
	int skinHeight; //height of skin texture
	int frameSize; //sizeof 1 frame in bytes

	int numSkins; //number of textures
	int numXYZ; //number of vertices
	int numST; //number of texture coords
	int numTriangles; //number of triangles
	int numGLcmds; //number of openGL commands
	int numFrames; //total frames

	int offsetSkins; //offset to skin names (64 bytes each)
	int offsetST; //offset to texture coords
	int offsetTriangles; //offset to traingles (faces)
	int offsetFrames; //offset to frames
	int offsetGlCmds; //offset to openGL commands
	int offsetEnd; //offset to end of file

}Md2Header, *PMd2Header;

#ifdef _MSC_VER
#pragma pack (pop)
#endif

/**
*	md2 model
*/
class modelMd2 : public meshCodec, public baseObject {

	int numVerts;
	int numTris;

	int numTexCoords;

	int curFrame;
	MD2_STATE state;

	texture* pkSkin;
	frame3d* pkFrames;
	Md2Face* pkFaces;
	Md2TexCoord* pkTexCoords;

	void calcNormal (Md2Vertex v1, Md2Vertex v2, Md2Vertex v3, Md2Vertex* out);

public:
	int frameSize;
	int numFrames;
	int startFrame;
	int endFrame;

	modelMd2 ();
	virtual ~modelMd2 ();

	virtual animatedMesh* createMesh (std::string fname);
	virtual ANIMATED_MESH_TYPE getMeshType () {return ANIMATED_MESH_MD2;}

	bool loadFile (file* pkModel, texture* pkTexture=0);
	void unload ();
	void modelMd2::render (int keyFrame);
	void setAnimationState (int i);
	void animate ();
	inline int getCurrentFrame () {return curFrame;}

	void render();
};

}

#endif

