/*****************************************************

	md2.cpp
		md2 codec

*****************************************************/

#include "md2.h"
#include <mgr/file/fileManager.h>
#include <core/kernel.h>
#include <windows.h>
#include <gl/gl.h>
#include <iostream>

namespace mercury {

// magic number "IDP2"
#define MD2_MAGIC (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')

// model version must be 8
#define MD2_VERSION 8

//! max frame count
#define MAX_MD2_FRAMES 199

//! number of precalculated normals
#define NUM_PRECALC_NORMALS 162

//! number of precalculated dot products
#define NUM_PRECALC_DOT_PRODUCTS 16

/**
*	Initialize
*/
modelMd2::modelMd2 ()
:
	numVerts (0),
	numTris (0),
	numFrames (0),
	state(IDLE),
	startFrame(0),
	endFrame(0),
	pkFrames(0),
	pkFaces(0),
	pkSkin(0),
	pkTexCoords(0),
	numTexCoords(0)
{

	type=BASEOBJECT_TYPE_SKYBOX;
}

/**
*	Release resources
*/
modelMd2::~modelMd2 () {

	unload ();
}

/**
*	Sets animation state
*/
void modelMd2::setAnimationState (int i){

	switch (i) {

		case IDLE:{
			startFrame=0;
			endFrame=39;
			break;
		}
		case RUN:{
			startFrame=40;
			endFrame=45;
			break;
		}
		case ATTACK:{
			startFrame=46;
			endFrame=53;
			break;
		}
		case PAIN1:{
			startFrame=54;
			endFrame=57;
			break;
		}
		case PAIN2:{
			startFrame=58;
			endFrame=61;
			break;
		}
		case PAIN3:{
			startFrame=62;
			endFrame=65;
			break;
		}
		case JUMP:{
			startFrame=66;
			endFrame=71;
			break;
		}
		case FLIPOFF:{
			startFrame=72;
			endFrame=83;
			break;
		}
		case SALUTE:{
			startFrame=84;
			endFrame=94;
			break;
		}
		case TAUNT:{
			startFrame=95;
			endFrame=111;
			break;
		}
		case WAVE:{
			startFrame=112;
			endFrame=122;
			break;
		}
		case POINT:{
			startFrame=123;
			endFrame=134;
			break;
		}
		case CROUCH_IDLE:{
			startFrame=135;
			endFrame=153;
			break;
		}
		case CROUCH_WALK:{
			startFrame=154;
			endFrame=159;
			break;
		}
		case CROUCH_ATTACK:{
			startFrame=160;
			endFrame=168;
			break;
		}
		case CROUCH_PAIN:{
			startFrame=169;
			endFrame=172;
			break;
		}
		case CROUCH_DEATH:{
			startFrame=173;
			endFrame=177;
			break;
		}
		case DEATH1:{
			startFrame=178;
			endFrame=183;
			break;
		}
		case DEATH2:{
			startFrame=184;
			endFrame=189;
			break;
		}
		case DEATH3:{
			startFrame=190;
			endFrame=197;
			break;
		}
		default:{
			startFrame=0;	//IDLE state
			endFrame=39;
			break;
		}
	}

	curFrame=startFrame;
}

/**
*	Calculate normal
*/
void modelMd2::calcNormal (Md2Vertex v1, Md2Vertex v2, Md2Vertex v3, Md2Vertex* out) {

	double v1x,v1y,v1z,v2x,v2y,v2z;
	double nx,ny,nz;
	double vLen;
 
	//! Calculate vectors
	v1x = v1.x - v2.x;
	v1y = v1.y - v2.y;
	v1z = v1.z - v2.z;
 
	v2x = v2.x - v3.x;
	v2y = v2.y - v3.y;
	v2z = v2.z - v3.z;
  
	//! Get cross product of vectors
	nx = (v1y * v2z) - (v1z * v2y);
	ny = (v1z * v2x) - (v1x * v2z);
	nz = (v1x * v2y) - (v1y * v2x);
 
	//! Normalise final vector
	vLen = sqrt( (nx * nx) + (ny * ny) + (nz * nz) );
 
	out->x = (float) (nx / vLen);
	out->y = (float) (ny / vLen);
	out->z = (float) (nz / vLen);
}

/**
*	Update animation frame
*/
void modelMd2::animate () {

	//running at 30fps
	static int counter=0;

	//! reset
	if (curFrame>=endFrame)
		curFrame=startFrame;

	//! render frame
	render(curFrame);

	if (++counter > 10) {
		curFrame++;
		counter=0;
	}
}

/**
*	Release resources
*/
void modelMd2::unload () {

	if (pkFrames) {

		if (pkFrames->vertexList) {

			delete [] pkFrames->vertexList;
			pkFrames->vertexList=0;
		}

		if (pkFrames->normals) {

			delete [] pkFrames->normals;
			pkFrames->normals=0;
		}

		delete [] pkFrames;
		pkFrames=0;
	}

	if (pkSkin) {

		delete pkSkin;
		pkSkin=0;
	}
}

/**
*	Render
*/
void modelMd2::render (int keyFrame) {

//need to render with BASEOBJECT_TYPE_STANDARD

	videoDriver* pkVideo = kernel::get()->getVideoDriver();
	camera* pkCam = kernel::get()->getSceneManager()->getCurrentCamera();

	matrix4<float> view;

	//! scale and position model in front of camera
	view.setScale(vector3d(0.01f,0.01f,0.01f));

	//! rotate model
	matrix4<float> rotateX, rotateZ;
	rotateX.xRotateRH(-89.5f);
//	rotateZ.yRotateRH(-90.0f);
	view *= rotateX * rotateZ;

	float height=6.0f;

	character* pkChar = kernel::get()->getCharacter ();
//	vector3d p = vector3d(10,-10+height,0);
	vector3d p = vector3d(1,-1,0);

	view.setTranslation(pkChar->getPosition() + p);

	//! set transform
	pkVideo->setTransform (TRANSFORM_MODELVIEW, view);

	material mat;
	mat.fog=true;
	mat.lighting=true;
	mat.ambient=color(254,254,254,255);
	mat.diffuse=color(0,0,0,0);
	mat.specular=color(200,200,200,255);
	mat.shininess=0;
	mat.backFaceCull=false;
	mat.frontFaceCull=false;

	if (pkSkin)
		pkVideo->setTexture (pkSkin);

	pkVideo->setMaterial(&mat);

  int Part;

  glBegin(GL_TRIANGLES);  
 
   for(Part=0;Part<numTris;++Part)
    {
		glNormal3f (pkFrames [keyFrame].normals [Part].x,
			pkFrames [keyFrame].normals [Part].y,
			pkFrames [keyFrame].normals [Part].z);

		if (pkSkin)
			glTexCoord2f (pkTexCoords[ pkFaces [Part].uv1].s,
				pkTexCoords[ pkFaces [Part].uv1].t);

		glVertex3f (pkFrames[keyFrame].vertexList[pkFaces[Part].p1].x,
			pkFrames[keyFrame].vertexList[pkFaces[Part].p1].y,
			pkFrames[keyFrame].vertexList[pkFaces[Part].p1].z);

		if (pkSkin)
			glTexCoord2f (pkTexCoords[ pkFaces [Part].uv2].s,
				pkTexCoords[ pkFaces [Part].uv2].t);

		glVertex3f (pkFrames[keyFrame].vertexList[pkFaces[Part].p2].x,
			pkFrames[keyFrame].vertexList[pkFaces[Part].p2].y,
			pkFrames[keyFrame].vertexList[pkFaces[Part].p2].z);

		if (pkSkin)
			glTexCoord2f (pkTexCoords[ pkFaces [Part].uv3].s,
				pkTexCoords[ pkFaces [Part].uv3].t);

		glVertex3f (pkFrames[keyFrame].vertexList[pkFaces[Part].p3].x,
			pkFrames[keyFrame].vertexList[pkFaces[Part].p3].y,
			pkFrames[keyFrame].vertexList[pkFaces[Part].p3].z);
    }  
 
  glEnd();
}

/**
*	Load file
*/
bool modelMd2::loadFile (file *pkFile,
						 texture* pkTexture) {

	unsigned char* buffer = 0;
	int fileLen = 0;

	//! sanity check
	if (! pkFile) {
		std::cout << "\n\rMd2 loadFile: none" << std::endl;
		return false;
	}

	//! check if file is opened
	if (!pkFile->isOpened()) {
		std::cout << "\n\rMd2 loadFile: not opened" << std::endl;
		return false;
	}

	//! get size of file
	fileLen = pkFile->getSize ();

	//! read entire file into memory
	buffer = new unsigned char [ fileLen+1 ];
	pkFile->read ((char*) buffer, fileLen );

	//! extract file header
	Md2Header* header = (Md2Header*) buffer;

	//! check if valid file
	if (header->magic != MD2_MAGIC)
		return false;

	//! store basic info
	numVerts = header->numXYZ;
	numFrames = header->numFrames;
	frameSize = header->frameSize;
	numTris = header->numTriangles;
	numTexCoords = header->numST;
	pkSkin = pkTexture;

	//! allocate frames
	pkFrames = new frame3d [numFrames];

	for (unsigned long i=0; i!=numFrames; ++i) {

		pkFrames[i].vertexList = new Md2Vertex [numVerts];
		pkFrames[i].normals = new Md2Vertex [numTris];

		if (!pkFrames[i].vertexList)
			std::cout << "ALLOC FAIL: VLIST" << std::endl;

		if (!pkFrames[i].normals)
			std::cout << "ALLOC FAIL: NLIST" << std::endl;
	}

	//! allocate faces
	pkFaces = new Md2Face [numTris];

	//! read faces
	memcpy (pkFaces, &buffer [header->offsetTriangles], numTris * sizeof (Md2Face) );

	//! allocate texture coords
	pkTexCoords = new Md2TexCoord [numTexCoords];
	Md2TexCoordIndex* texCoordMd2 = new Md2TexCoordIndex [numTexCoords];

	//! read in texture coords
	memcpy (texCoordMd2, &buffer [header->offsetST], numTexCoords * sizeof (Md2TexCoordIndex) );

	//! uncompress textures
	for (int i=0; i<numTexCoords; i++) {

		pkTexCoords[i].s = ((float) texCoordMd2[i].s) / header->skinWidth;
		pkTexCoords[i].t = ((float) texCoordMd2[i].t) / header->skinHeight;
	}

	delete [] texCoordMd2;
	texCoordMd2=0;

	//! allocate vertex list
	Md2FramePoint* pkVertices = new Md2FramePoint [numVerts];

	for (unsigned long i=0; i!=numFrames; ++i) {

		Md2Frame frame;

		//! get frame data
		memcpy (&frame, &buffer [ header->offsetFrames + (frameSize * i) ], sizeof (Md2Frame) );

		//! get vertex data
		memcpy (pkVertices, &buffer [ header->offsetFrames + (frameSize * i) + sizeof (Md2Frame)],  numVerts * sizeof (Md2FramePoint) );

		//! extract all vertices in frame
		for (unsigned long j = 0; j != numVerts; ++j) {

			pkFrames[i].vertexList[j].x = pkVertices[j].v[0] * frame.scale[0] + frame.translate[0];
			pkFrames[i].vertexList[j].y = pkVertices[j].v[1] * frame.scale[1] + frame.translate[1];
			pkFrames[i].vertexList[j].z = pkVertices[j].v[2] * frame.scale[2] + frame.translate[2];
		}
	}

	delete [] pkVertices;
	pkVertices = 0;

	//! calculate normals
	for (unsigned long frame=0;frame!=numFrames;frame++) {

		for (unsigned long face=0;face!=numTris; face++) {

			Md2Vertex norm;

			calcNormal (pkFrames [frame].vertexList [pkFaces [face].p1],
						pkFrames [frame].vertexList [pkFaces [face].p2],
						pkFrames [frame].vertexList [pkFaces [face].p3],
						&norm);

			pkFrames [frame].normals [face].x = norm.x;
			pkFrames [frame].normals [face].y = norm.y;
			pkFrames [frame].normals [face].z = norm.z;
		}
	}

	delete [] buffer;

	return true;
}

animatedMesh* modelMd2::createMesh (std::string fname) {

	fileManager* fileMgr = kernel::get()->getFileManager ();

	if (fileMgr) {

		file* pkFile = fileMgr->open (fname);
		if (pkFile) {

			animatedMesh* pkMesh = 0;//new animatedMeshMd2;

//			if (pkMesh)
//				;
;//				loadFile (pkFile, pkMesh);

			fileMgr->close (pkFile);
			return pkMesh;
		}
	}

	return 0;
}

/**
*	derived from baseObject
*/
void modelMd2::render () {

	animate ();
}

}
