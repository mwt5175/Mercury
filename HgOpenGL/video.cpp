/*****************************************************

	vidOgl.h
		OpenGL Video Driver API

*****************************************************/

#define MERCURY_SDL

#include <iostream>

#include "video.h"

using namespace hgm;
using namespace hgg;

namespace hgogl {

/**
*	Constructor
*/
HgVideoDriverOGL::HgVideoDriverOGL ()
	: wireframe (false),fullscreen (false)
{
}

HgVideoDriverOGL::~HgVideoDriverOGL () {
	stop ();
}

void HgVideoDriverOGL::stop() {

}

/**
*	Clear depth buffer
*/
void HgVideoDriverOGL::clearZBuffer () {
	glClear (GL_DEPTH_BUFFER_BIT);
}

/**
*	Get driver information
*/
void HgVideoDriverOGL::getDriverInfo () {
	const unsigned char* str = glGetString(GL_VERSION);
	std::cout << "OPENGL VERSION: " << str << std::endl;
}

/**
*	Start next frame
*/
HGVOID HgVideoDriverOGL::startFrame (HgColor back, bool clear, bool clearDepth) {

	HgColorf col ( back );
	glClearColor ( col.getRed(), col.getGreen(), col.getBlue(), col.getAlpha() );

	int value = 0;

	if (clear)
		value |= GL_COLOR_BUFFER_BIT;

	if (clearDepth)
		value |= GL_DEPTH_BUFFER_BIT;

	value |= GL_STENCIL_BUFFER_BIT;

	glClear( value );
}

void HgVideoDriverOGL::endFrame () {
	//SDL_GL_SwapBuffers();
}

/**
*	Initialize video driver
*/
bool HgVideoDriverOGL::start () {

	return genericStart ();
}

#if 0
	SDL_Init(SDL_INIT_EVERYTHING);

	sdlSurface = SDL_SetVideoMode(width,height,bpp,SDL_HWSURFACE | SDL_RESIZABLE | (fullscreen?SDL_FULLSCREEN:0));
	if(!sdlSurface)
		return false;

	SDL_WM_SetCaption(caption.c_str(),NULL);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#endif

HgRect& HgVideoDriverOGL::getViewport () {
	HgRect rc;
	return rc;
}

void HgVideoDriverOGL::setViewport (HgRect& view) {

	glViewport (view.topleft.x, view.topleft.y,	//x, y
		view.bottomright.x - view.topleft.x,	//width
		view.bottomright.y - view.topleft.y );	//height
}

HgIndexBufferSharedPtr HgVideoDriverOGL::newIndexBuffer (HgIndexType type, size_t numIndices) {
	HgIndexBufferSharedPtr s;
	return s;
}

HgVertexBufferSharedPtr HgVideoDriverOGL::newVertexBuffer (size_t count, size_t size) {
	HgVertexBufferSharedPtr v;
	return v;
}

HgTextureSharedPtr HgVideoDriverOGL::newTexture () {
	HgTextureSharedPtr t;
	return t;
}

HgShaderProgramSharedPtr HgVideoDriverOGL::newShaderProgram () {
	HgShaderProgramSharedPtr p;
	return p;
}

/**
*	Platform independent start routine
*/
bool HgVideoDriverOGL::genericStart () {

	//! generic setup
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(1.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Doesnt work out how it shoudl on skybox :(
	glEnable(GL_CLAMP_TO_EDGE);

	setViewport (HgRect(0,0,640,48));

//	setViewport (HgRect(0,0,sdlSurface->w, sdlSurface->h));

	return true;
}

} // namespace hgogl



#if 0

/**
*	Set rendering mode states (internal use)
*/
void videoDriverOGL::setMode (RENDER_MODE m) {

	if (m == RENDER_2D) {

		//! reset projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//! reset view matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//! disable 3d-specific items
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_FOG);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);

		glDisable (GL_TEXTURE_2D);

		//! needed for proper render
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//! enable back-face culling and polygon fill
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		mode = RENDER_2D;
	}
	else {

		GLfloat glMatrix[16];

		//! reload worldview matrix
		CreateGLMatrix (glMatrix, worldView );
		glMatrixMode (GL_MODELVIEW);
		glLoadMatrixf (glMatrix);

		//! reload projection matrix
		CreateGLMatrix (glMatrix, projection);
		glMatrix[12] *= -1.0f;   // OpenGL uses right-handed coord system, so we need to flip z
		glMatrixMode (GL_PROJECTION);
		glLoadMatrixf (glMatrix);

		//! all other settings are enabled as-needed (ie lighting and fog),
		//! so we dont need to here
		mode = RENDER_3D;
	}
}


/**
*	Loads available and supported extensions
*/
void videoDriverOGL::loadExtensions () {

	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTextureUnitsExt);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropyExt);
}

/**
*	Stops the video driver
*/
void videoDriverOGL::stop () {

	//! close driver
	close ( true );

	if (glIsEnabled (GL_BLEND) )
		glDisable (GL_BLEND);

	if (glIsEnabled (GL_LIGHTING) )
		glDisable (GL_LIGHTING);

	if (glIsEnabled (GL_FOG) )
		glDisable (GL_FOG);

	if (glIsEnabled (GL_DEPTH_TEST) )
		glDisable (GL_DEPTH_TEST);

	if (glIsEnabled (GL_TEXTURE_2D) )
		glDisable (GL_TEXTURE_2D);

	if(glIsEnabled (GL_STENCIL_TEST) )
		glDisable (GL_STENCIL_TEST);

#ifdef MERCURY_WINDOWS
	//! switch back to window mode
	if (fullscreen)
		ChangeDisplaySettings (NULL, 0);

	//! reset current GL resource context
	wglMakeCurrent (NULL, NULL);

	//! unregister window class
	UnregisterClass ("WNDCLASS", GetModuleHandle (0) );

	//! close window
	CloseWindow ( (HWND) lpSurface.lpData );
#elif defined MERCURY_SDL
	SDL_Quit();
#endif
}

/**
*	Sets the perspective in view frustum
*/
void videoDriverOGL::perspective (const float fVisionY, float fAspect, const float fZnear, const float fZfar ) {

	float   fHalfClipX;
	float   fHalfClipY;

	fHalfClipY = tan ( (fVisionY / 2) / 180 * 3.1415926535897932384626433832795f ) * fZnear;
	fHalfClipX = fHalfClipY * fAspect;

	glFrustum( -fHalfClipX, fHalfClipX, -fHalfClipY, fHalfClipY, fZnear, fZfar );
}

/**
*	Sets the viewport. All rendering will be
*	done on the surface within the viewport region only
*/
void	videoDriverOGL::setViewport (rect<int> view) {

	glViewport (view.topleft.x, view.topleft.y,	//x, y
		view.bottomright.x - view.topleft.x,	//width
		view.bottomright.y - view.topleft.y );	//height

	viewport = view;
}

/**
*	Returns the current viewport
*/
rect<int> videoDriverOGL::getViewport () {

	return viewport;
}

/**
*	Sets the view frustum
*/
void videoDriverOGL::setFrustum (frustum f) {

	glFrustum (f.getLeft (), f.getRight (), f.getBottom (), f.getTop(),
		f.getNear(), f.getFar () );
}

/**
*	Starts blending
*/
void videoDriverOGL::startBlend () {

	if (! glIsEnabled (GL_BLEND) ) {

		//! set blend function
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//! enable
		glEnable (GL_BLEND);
	}
}

/**
*	Stops blending
*/
void videoDriverOGL::stopBlend () {

	//! disable blending
	if ( glIsEnabled (GL_BLEND) )
		glDisable (GL_BLEND);
}

/**
*	Renders 2d rectangle
*/
void videoDriverOGL::rectangle2d (color col, rect<int> rc) {

	//! enable for 2d rendering
	if (mode!=RENDER_2D)
		setMode (RENDER_2D);

	int width=0,height=0;

	//! should work on making this easier here
#ifdef MERCURY_WINDOWS
	width=lpSurface->width;
	height=lpSurface->height;
#elif defined MERCURY_SDL
	width=sdlSurface->w;
	height=sdlSurface->h;
#endif

	signed int xPlus = -(width >> 1);
	float xFact = 1.0f / (width >> 1);

	signed int yPlus = height - (height >> 1);
	float yFact = 1.0f / (height >> 1);

	//! set color
	colorf c (col);
	glColor4f ( c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );

	//! draw the rectangle
	glRectf((float)(rc.topleft.x + xPlus) * xFact,
		(float)(yPlus - rc.topleft.y) * yFact,
		(float)(rc.bottomright.x + xPlus) * xFact,
		(float)(yPlus - rc.bottomright.y) * yFact);

	//! reset color
	glColor4f (1,1,1,1);
}

/**
*	Renders 2d rectangle with different colors
*/
void videoDriverOGL::rectangle2d (	color col1, color col2,
							color col3, color col4,
							rect<int> rc) {
	colorf c;

	//! enable for 2d rendering
	if (mode!=RENDER_2D)
		setMode (RENDER_2D);

	int width=0,height=0;

#ifdef MERCURY_WINDOWS
	width=lpSurface->width;
	height=lpSurface->height;
#elif defined MERCURY_SDL
	width=sdlSurface->w;
	height=sdlSurface->h;
#endif

	signed int xPlus = -(width >> 1);
	float xFact = 1.0f / (width >> 1);

	signed int yPlus = height - (height >> 1);
	float yFact = 1.0f / (height >> 1);

	glBegin (GL_QUADS);
		c.fromColor (col1);
			glColor4f (c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );
			glVertex2f ( (rc.topleft.x + xPlus) * xFact, (yPlus - rc.topleft.y) * yFact);
		c.fromColor (col2);
			glColor4f (c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );
			glVertex2f ( (rc.bottomright.x + xPlus) * xFact, (yPlus - rc.topleft.y) * yFact);
		c.fromColor (col3);
			glColor4f (c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );
			glVertex2f ( (rc.bottomright.x + xPlus) * xFact, (yPlus - rc.bottomright.y) * yFact);
		c.fromColor (col4);
			glColor4f (c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );
			glVertex2f ( (rc.topleft.x + xPlus) * xFact, (yPlus - rc.bottomright.y) * yFact);
	glEnd ();

	//! reset color
	glColor4f (1,1,1,1);
}

/**
*	Renders a 2d line
*/
void videoDriverOGL::line2d (color col, point2d<float> pt, point2d<float> pt2) {

	//! enable for 2d rendering
	if (mode!=RENDER_2D)
		setMode (RENDER_2D);

	int width=0,height=0;

#ifdef MERCURY_WINDOWS
	width=lpSurface->width;
	height=lpSurface->height;
#elif defined MERCURY_SDL
	width=sdlSurface->w;
	height=sdlSurface->h;
#endif

	signed int xPlus = -(width >> 1);
	float xFact = 1.0f / (width >> 1);

	signed int yPlus = height - (height >> 1);
	float yFact = 1.0f / (height >> 1);

	colorf c (col);

	glBegin(GL_LINES);
		glColor4f ( c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );
		glVertex2f ( (pt.x + xPlus) * xFact, (yPlus - pt.y) * yFact );
		glVertex2f ( (pt2.x + xPlus) * xFact, (yPlus - pt2.y) * yFact );
	glEnd ();

	//! reset color
	glColor4f (1,1,1,1);
}

/**
*	Renders a 3d line
*/
void videoDriverOGL::line3d (color col, point3d<float> pt, point3d<float> pt2) {

	colorf c (col);
	glColor4f ( c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );

	glBegin(GL_LINES);
		glVertex3f ( pt.x, pt.y, pt.z );
		glVertex3f ( pt2.x, pt2.y, pt2.z );
	glEnd ();

	//! reset color
	glColor4f (1,1,1,1);
}

/**
*	Renders a 2d or 3d triangle in a single color
*/
void videoDriverOGL::triangle (vertex3d v1, vertex3d v2, vertex3d v3) {

	colorf c;

	glBegin (GL_TRIANGLES);
		c.fromColor (v1.col );
			glColor4f ( c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );
			glVertex3f (v1.pos.getX(), v1.pos.getY(), v1.pos.getZ () );
		//	glVertex2f (v1.pos.getX(), v1.pos.getY());
		c.fromColor (v2.col );
			glColor4f ( c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );
			glVertex3f (v2.pos.getX(), v2.pos.getY(), v2.pos.getZ () );
		//	glVertex2f (v1.pos.getX(), v1.pos.getY());
		c.fromColor (v3.col );
			glColor4f ( c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );
			glVertex3f (v3.pos.getX(), v3.pos.getY(), v3.pos.getZ () );
		//	glVertex2f (v1.pos.getX(), v1.pos.getY());
	glEnd ();

	//! reset color
	glColor4f (1,1,1,1);
}

/**
*	Plots a pixel
*/
void videoDriverOGL::plotPixel (color col, point2d<float> pt) {

	colorf c (col);
	glColor4f ( c.getRed (), c.getGreen(), c.getBlue(), c.getAlpha () );

	glBegin (GL_POINTS);
		glVertex2f (pt.x, pt.y);
	glEnd ();

	//! reset color
	glColor4f (1,1,1,1);
}

/**
*	Get a pixel
*/
int videoDriverOGL::getPixel (IMAGE_FORMAT Format, point2d<int> pt) {

	int format=GL_RGBA;
	int pixel = 0;

	switch ( Format ) {

		case IMAGE_FORMAT_8888RGBA:
			format = GL_RGBA;
			break;
		case IMAGE_FORMAT_888RGB:
			format = GL_RGB;
			break;
	}

	glReadPixels (pt.x, pt.y, 1, 1,
		format, GL_UNSIGNED_BYTE, &pixel);

	return pixel;
}

/**
*	Sets pixel size
*/
void videoDriverOGL::pixelSize (float size) {

	glPointSize ( size );
}

/**
*	Render stencil shadow
*/
void videoDriverOGL::stencilShadow (bool clear) {

#if 0
	if (!glIsEnabled (GL_STENCIL_TEST))
		glEnable (GL_STENCIL_TEST);

	if (clear)
		glClear (GL_STENCIL_BUFFER_BIT);

	// store attributes
	glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );
	glPushMatrix();

	glDisable( GL_LIGHTING );
	glDepthMask(GL_FALSE);
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_STENCIL_TEST );

	glFrontFace( GL_CCW );
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glStencilFunc(GL_NOTEQUAL, 0, 0xFFFFFFFFL);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glDisable(GL_FOG);

	glLoadIdentity();

	//! draw A shadowing rectangle covering entire screen
	glBegin(GL_TRIANGLE_STRIP);

	glColor4ub (255, 0, 0, 125 );
	glVertex3f(-10.1f, 10.1f,0.90f);

	glColor4ub (255, 0, 0, 125 );
	glVertex3f(-10.1f,-10.1f,0.90f);

	glColor4ub (255, 0, 0,125 );
	glVertex3f( 10.1f, 10.1f,0.90f);

	glColor4ub (255, 0, 0, 125 );
	glVertex3f( 10.1f,-10.1f,0.90f);

	glEnd();
	glFrontFace( GL_CW );

	// restore settings
	glPopMatrix();
	glPopAttrib();
#endif
}

/**
*	Render an indixed triangle list
*/
void videoDriverOGL::indexedTriList (vertex3d* verts,
		unsigned int* indices,
		int numTris) {

//	float colorBuffer[(2*3)*4];

//	for (int i=0; i<(2*3)*4;i++)
//		colorBuffer[i]=1;

	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
//	glEnableClientState (GL_COLOR_ARRAY);

	glVertexPointer (3, GL_FLOAT, sizeof (vertex3d), &verts[0].pos.x );
	glNormalPointer (GL_FLOAT, sizeof (vertex3d), &verts[0].normal.x );
	glTexCoordPointer (2, GL_FLOAT, sizeof (vertex3d), &verts[0].texcoords.x );
//	glColorPointer (4,GL_UNSIGNED_INT,sizeof (vertex3d), &verts[0].col.col);
//	glColorPointer(4, GL_FLOAT, sizeof(float), &colorBuffer[0] );

	glDrawElements (GL_TRIANGLES, numTris * 3, GL_UNSIGNED_INT, indices);

	glFlush ();

//	glDisableClientState (GL_COLOR);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_VERTEX_ARRAY);
}

/**
*	Render an indixed triangle fan
*/
void videoDriverOGL::indixedTriFan (vertex3d* verts,
		unsigned int* indices,
		int numTris) {

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState (GL_NORMAL_ARRAY);
		glEnableClientState (GL_TEXTURE_COORD_ARRAY);

		glVertexPointer( 3, GL_FLOAT, sizeof (vertex3d), &verts[0].pos.x );
		glNormalPointer (GL_FLOAT, sizeof (vertex3d), &verts[0].normal.x );
		glTexCoordPointer (2, GL_FLOAT, sizeof (vertex3d), &verts[0].texcoords.x );

		//triangle count is always 3 + (numTris-1)
		glDrawElements( GL_TRIANGLE_FAN, 3 + (numTris-1), GL_UNSIGNED_INT, indices );

		glFlush ();

		glDisableClientState (GL_TEXTURE_COORD_ARRAY);
		glDisableClientState (GL_NORMAL_ARRAY);
		glDisableClientState (GL_VERTEX_ARRAY);
}

/**
*	Render an indixed triangle strip
*/
void videoDriverOGL::indixedTriStrip (vertex3d* verts,
		unsigned int* indices,
		int numTris) {

		glColor3f(0,0,0);

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState (GL_NORMAL_ARRAY);
		glEnableClientState (GL_TEXTURE_COORD_ARRAY);

		glVertexPointer( 3, GL_FLOAT, sizeof (vertex3d), &verts[0].pos.x );
		glNormalPointer (GL_FLOAT, sizeof (vertex3d), &verts[0].normal.x );
		glTexCoordPointer (2, GL_FLOAT, sizeof (vertex3d), &verts[0].texcoords.x );

		//! First 3 vertices=1 triangle; every other vertex=1 additional triangle

		//triangle count is always 3 + (numTris-1)
		glDrawElements( GL_TRIANGLE_STRIP, 3 + (numTris-1), GL_UNSIGNED_INT, indices );

		glFlush ();

		glDisableClientState (GL_TEXTURE_COORD_ARRAY);
		glDisableClientState (GL_NORMAL_ARRAY);
		glDisableClientState (GL_VERTEX_ARRAY);
}

/**
*	Render an indixed triangle list
*/
void videoDriverOGL::indexedTriList2 (float* verts,
		unsigned int* indices,
		float* texcoords,
		int numTris) {

	glEnableClientState (GL_VERTEX_ARRAY);
//	glEnableClientState (GL_TEXTURE_COORD_ARRAY);

	glVertexPointer (3, GL_FLOAT, 0, &verts[0] );
//	glTexCoordPointer (2, GL_FLOAT, 0, &texcoords[0]);

	glDrawElements (GL_TRIANGLES, numTris * 3, GL_UNSIGNED_INT, indices);

	glFlush ();

//	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_VERTEX_ARRAY);
}

/**
*	Adds a light
*/
void videoDriverOGL::addLight (light l) {

	if (numLights >= GL_MAX_LIGHTS)
		return;

	if (! glIsEnabled (GL_LIGHTING) )
		glEnable (GL_LIGHTING);

	float pos [4];
	float diffuse [4];
	float ambient [4];
	float specular [4];
	colorf c;

	if (l.type == LIGHT_DIRECTIONAL) {

		float direction[3];
		direction [0] = l.direction.x;
		direction [1] = l.direction.y;
		direction [2] = l.direction.z;

		pos[0] = l.position.x;
		pos[1] = l.position.y;
		pos[2] = l.position.z;
		pos[3] = 0.0f;	//must be 0 for directional light source

		glLightfv (GL_LIGHT0 + numLights , GL_SPOT_DIRECTION, direction);
	}
	else {

		pos[0] = l.position.x;
		pos[1] = l.position.y;
		pos[2] = l.position.z;
		pos[3] = 1.0f;	//position of the light source
	}

	c.fromColor (l.diffuseColor);
	diffuse[0] = c.getRed ();
	diffuse[1] = c.getGreen ();
	diffuse[2] = c.getBlue ();
	diffuse[3] = c.getAlpha ();

	c.fromColor (l.ambientColor);
	ambient[0] = c.getRed ();
	ambient[1] = c.getGreen ();
	ambient[2] = c.getBlue ();
	ambient[3] = c.getAlpha ();

	c.fromColor (l.specularColor);
	specular[0] = c.getRed ();
	specular[1] = c.getGreen ();
	specular[2] = c.getBlue ();
	specular[3] = c.getAlpha ();

	glLightfv (GL_LIGHT0 + numLights, GL_AMBIENT, ambient);
	glLightfv (GL_LIGHT0 + numLights, GL_DIFFUSE, diffuse);
	glLightfv (GL_LIGHT0 + numLights, GL_SPECULAR, specular);

	glLightfv (GL_LIGHT0 + numLights , GL_POSITION, pos);

//glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,14.0);

	glEnable (GL_LIGHT0 + numLights);

	numLights++;
}

/**
*	Sets the overall ambient light color
*/
void videoDriverOGL::setAmbientLight (color col) {
	//! 0.2,0.2,0.2,1.0 is OGL default
	colorf c(col);
	float data[4] = {c.getRed(), c.getBlue(), c.getBlue(), c.getAlpha() };

	glLightModelfv (GL_LIGHT_MODEL_AMBIENT, data);
}

/**
*	Returns maximum number of lights allowed
*/
unsigned int videoDriverOGL::getMaxLights () {

	return GL_MAX_LIGHTS;
}

/**
*	Returns number of lights
*/
unsigned int videoDriverOGL::getNumLights () {

	return numLights;
}

/**
*	Disables all lights
*/
void videoDriverOGL::disableAllLights () {

	for (unsigned int i = 0; i < numLights; i++)
		glDisable (GL_LIGHT0 + numLights);

	numLights = 0;
}

/**
*	Sets the fog
*/
void videoDriverOGL::setFog (fog fogInfo) {

	colorf fogColor;
	fogColor.fromColor (fogInfo.fogColor);

	float col[4] = {fogColor.getRed(),
		fogColor.getGreen(),
		fogColor.getBlue(),
		fogColor.getAlpha() };

	switch (fogInfo.fogType) {

		case FOG_LINEAR: {
			glFogi (GL_FOG_MODE, GL_LINEAR);
			break;
		}
		case FOG_EXP: {
			glFogi (GL_FOG_MODE, GL_EXP);
			break;
		}
		case FOG_EXP_2: {
			glFogi (GL_FOG_MODE, GL_EXP2);
			break;
		}
	}

	glFogfv (GL_FOG_COLOR, col);
	glFogf (GL_FOG_DENSITY, fogInfo.density);
	glFogf (GL_FOG_START, fogInfo.start);
	glFogf (GL_FOG_END, fogInfo.end);
	glHint (GL_FOG_HINT, GL_DONT_CARE);

	if (! glIsEnabled (GL_FOG) )
		glEnable (GL_FOG);
}

/**
*	Sets solid/wireframe rendering
*/
void videoDriverOGL::setWireframe (bool mode) {

	if (mode==true)
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode (GL_BACK, GL_FILL);

	wireframe = mode;
}

/**
*	Test if wireframe render
*/
bool videoDriverOGL::isWireframe () {

	return wireframe;
}

/**
*	Converts matrix to OpenGL compatable matrix
*/
void videoDriverOGL::CreateGLMatrix( GLfloat glMatrix[16], const matrix4<float>& m) {

	int i = 0;
	for (int r=0; r<4; ++r)
		for (int c=0; c<4; ++c) {
			glMatrix[i++] =  m (c,r);
		}
}

/**
*	Sets transformation
*/
void videoDriverOGL::setTransform (TRANSFORMATION transform, matrix4<float> mat) {

	GLfloat glMatrix[16];

	currentTransform = transform;

	switch (currentTransform) {

		case TRANSFORM_WORLD: {

			worldView = mat;
			CreateGLMatrix (glMatrix, mat );
			glMatrixMode (GL_MODELVIEW);
			glLoadMatrixf (glMatrix);
			break;
		}

		case TRANSFORM_MODELVIEW: {

			modelview = mat;
			CreateGLMatrix (glMatrix, worldView * mat );
			glMatrixMode (GL_MODELVIEW);
			glLoadMatrixf (glMatrix);
			break;
		}

		case TRANSFORM_PROJECTION: {

			projection = mat;
			CreateGLMatrix (glMatrix, mat);
			glMatrix[12] *= -1.0f;   // OpenGL uses right-handed coord system, so we need to flip z //
			glMatrixMode (GL_PROJECTION);
			glLoadMatrixf (glMatrix);
			break;
		}
	}
}

/**
*	Get transform
*/
matrix4<float>& videoDriverOGL::getTransform (TRANSFORMATION transform) {

	switch (transform) {

		case TRANSFORM_WORLD:
			return worldView;
		case TRANSFORM_PROJECTION:
			return projection;
		case TRANSFORM_MODELVIEW:
		default:
			return modelview;
	}
}

/**
*	Renders a 2d image
*	todo: render partial images and clipping rects
*/
void videoDriverOGL::image2d (point2d<int> pos, texture* pkImage) {

	//! enable for 2d rendering
	if (mode!=RENDER_2D)
		setMode (RENDER_2D);

	int width=0,height=0;

	rect<int> rc;
	rc.topleft.x=pos.x;
	rc.topleft.y=pos.y;
	rc.bottomright.x=pos.x+pkImage->getWidth();
	rc.bottomright.y=pos.y+pkImage->getHeight();

#ifdef MERCURY_WINDOWS
	width=lpSurface->width;
	height=lpSurface->height;
#elif defined MERCURY_SDL
	width=sdlSurface->w;
	height=sdlSurface->h;
#endif

	signed int xPlus = -(width >> 1);
	float xFact = 1.0f / (width >> 1);

	signed int yPlus = height - (height >> 1);
	float yFact = 1.0f / (height >> 1);

	setTexture (pkImage);

	glBegin (GL_QUADS);
	glTexCoord2f(0,0);
			glVertex2f ( (rc.topleft.x + xPlus) * xFact, (yPlus - rc.topleft.y) * yFact);
	glTexCoord2f(1,0);
			glVertex2f ( (rc.bottomright.x + xPlus) * xFact, (yPlus - rc.topleft.y) * yFact);
	glTexCoord2f(1,1);
			glVertex2f ( (rc.bottomright.x + xPlus) * xFact, (yPlus - rc.bottomright.y) * yFact);
	glTexCoord2f(0,1);
			glVertex2f ( (rc.topleft.x + xPlus) * xFact, (yPlus - rc.bottomright.y) * yFact);
	glEnd ();

	//! reset color
	glColor4f (1,1,1,1);
}

/**
*	Reads screen data
*/
void videoDriverOGL::screenshot (IMAGE_FORMAT Format, rect<int> rc, unsigned char* p) {

	if (p) {

			int format=GL_RGBA;

			switch ( Format ) {

				case IMAGE_FORMAT_8888RGBA:
					format = GL_RGBA;
					break;
				case IMAGE_FORMAT_888RGB:
					format = GL_RGB;
					break;
			}

		glReadPixels (rc.topleft.x, rc.topleft.y,
			rc.bottomright.x - rc.topleft.x, rc.bottomright.y - rc.topleft.y,
			format, GL_UNSIGNED_BYTE, p);
	}
}

/**
*	Create a device texture
*/
texture* videoDriverOGL::createTexture () {

	return new texture2dOGL();
}

/**
*	Gets a texture from file
*/
texture* videoDriverOGL::getTexture (std::string pkFileName,
									 bool genMipMaps) {

	return 0;
#if 0
	texture* pkTexture=0;

	kernel* pkKernel = kernel::get();
	if (!pkKernel)
		return 0;

	fileManager* pkFileMngr = pkKernel->getFileManager ();
	if (!pkFileMngr)
		return 0;

	file* pkFile = pkFileMngr->open (pkFileName);
	if (!pkFile)
		return 0;

	std::string ext = pkFile->getFileExtension ();

	if (ext=="tga" || ext=="TGA") {

		imageCodecTGA tga;
		image* pkImage = tga.load (pkFile);

		if (!pkImage) {

			pkFileMngr->close (pkFile);
			return 0;
		}

		if (t==TEXTURE_2D)
			pkTexture = new textureOGL (pkImage,false);
		else if (t==TEXTURE_3D)
			pkTexture = new textureOGL_3D (pkImage);

		if (!pkTexture) {

			pkFileMngr->close (pkFile);
			tga.free (pkImage);
			return 0;
		}
	}
	else
		printf ("\n\r*** warning: Unsupported image %s type: '%s'",
		pkFile->getFilename().c_str(), ext.c_str() );

	pkFileMngr->close (pkFile);

	return pkTexture;
#endif
}

/**
*	Releases a texture
*/
void videoDriverOGL::deleteTexture (texture* pkTexture) {

	if (pkTexture)
		delete pkTexture;
}

/**
*	Sets current texture
*/
void videoDriverOGL::setTexture (texture* pkTexture) {
#if 0

	if (pkTexture) {

		if (pkTexture->getType()==TEXTURE_2D) {

			//! enable textures if needed
			if (! glIsEnabled (GL_TEXTURE_2D) )
				glEnable (GL_TEXTURE_2D);

			//! bind the texture
			glBindTexture (GL_TEXTURE_2D, pkTexture->getId () );
		}
		else if (pkTexture->getType()==TEXTURE_3D) {

			//! enable textures if needed
			if (! glIsEnabled (GL_TEXTURE_3D) )
				glEnable (GL_TEXTURE_3D);

			//! bind the texture
			glBindTexture (GL_TEXTURE_3D, pkTexture->getId () );
		}

	}else {

		if (pkTexture->getType()==TEXTURE_2D) {

			//! disable textures
			if ( glIsEnabled (GL_TEXTURE_2D) )
				glDisable (GL_TEXTURE_2D);
		}
		else if (pkTexture->getType()==TEXTURE_3D) {

			//! disable textures
			if ( glIsEnabled (GL_TEXTURE_3D) )
				glDisable (GL_TEXTURE_3D);
		}
	}
#endif
}

/**
*	Enable 2d mode
* 
* 2 right - left 0 0 t x
* 0                2                top - bottom 0
* t                y                0            0
* -2               farVal - nearVal t            z
* 0                0                0            1
*/
void videoDriverOGL::enable2D () {

	int vPort[4];

	glGetIntegerv(GL_VIEWPORT, vPort);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

/**
*	Disable 2d mode
*/
void videoDriverOGL::disable2D () {

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

/**
*	Sets global fog state
*/
void videoDriverOGL::setGlobalFogState(bool b) {

	gFog=b;
	if (b==true)
		glEnable (GL_FOG);
	else
		glDisable (GL_FOG);
}

/**
*	Screen to object coordinates (implements gluUnProject)
*	reference http://www.opengl.org/wiki/GluProject_and_gluUnProject_code
*/
vector3d videoDriverOGL::unProject (vector3d screenPos)  {

	vector3d pos;

	//! get current viewport
	int viewport[4];
	matrix4<float> projMat;
	matrix4<float> modelMat;

	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetFloatv (GL_MODELVIEW_MATRIX, modelMat.m);
	glGetFloatv (GL_PROJECTION_MATRIX, projMat.m);

	//! calculate for inverting a matrix; projection * modelview
	matrix4<float> A = projMat * modelMat;

	//! compute inverse of matrix
	matrix4<float> m;
	A.inverse (m);  // m = inverse(A)

	//! transformation of normalized coordinates
	float in[4];
	in[0] = (screenPos.x - (float)viewport[0]) / (float)viewport[2] * 2.0f - 1.0f;
	in[1] = (screenPos.y - (float)viewport[1]) / (float)viewport[3] * 2.0f - 1.0f;
	in[2] = 2.0f * screenPos.z - 1.0f;
	in[3] = 1.0f;

	//! object coordinates (out = m * in)
	matrix4<float> out;
	out[0] = m.m11 * in[0] + m.m21 * in[1] + m.m31 * in[2] + m.m41 * in[3];
	out[1] = m.m12 * in[0] + m.m22 * in[1] + m.m32 * in[2] + m.m42 * in[3];
	out[2] = m.m13 * in[0] + m.m23 * in[1] + m.m33 * in[2] + m.m43 * in[3];
	out[3] = m.m14 * in[0] + m.m24 * in[1] + m.m34 * in[2] + m.m44 * in[3];

	if (out[3]==0.0f) {
		std::cout << "Divide by 0" << std::endl;
		return screenPos;
	}
	out[3]=1.0f / out[3];

	//Clip Coordinate X, Y, and Z are divided by Clip Coordinate W
	//to produce Normalized Device Coordinates
	pos.x = out[0] * out[3];
	pos.y = out[1] * out[3];
	pos.z = out[2] * out[3];

	std::cout << "screenToObject [" << pos.x << "," << pos.y << "," << pos.z << "]" << std::endl;

	return screenPos;
}

/**
*	Return screen coords from world coordinates
*/
point2d<int> videoDriverOGL::worldToScreen (vector3d worldPos) {

	matrix4<float> modelview;
	matrix4<float> currentProjection;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview.m);
	glGetFloatv(GL_PROJECTION_MATRIX, currentProjection.m);

	vector3d res;

	matrix4<float> m = currentProjection * modelview;
	res.x = m.m[0] * worldPos.x;
	res.y = m.m[1] * worldPos.y;
	res.z = m.m[2] * worldPos.z;

	float w=1.0f;
	res.x = 640 * (res.x / w + 1.0f) * 0.5f;
	res.y = 480 * (res.y / w + 1.0f) * 0.5f;
	res.z = (res.x / w + 1.0f) * 0.5f;

	std::cout << "worldToScreen [" << res.x << "," << res.y << "]" << std::endl;

	point2d<int> screen;
	return screen;
}

/**
*	Sets current material
*/
void videoDriverOGL::setMaterial (material* pkMaterial) {

	if ( pkMaterial ) {

		//! update states flag
		bool updateStates = false;

		//! if no current material set, set it to this one
//		if (!pkCurrentMaterial) {

			pkCurrentMaterial = pkMaterial;
			updateStates = true;
//		}

		//! if this is the same material, no need to change anything
//		if (pkMaterial == pkCurrentMaterial)
//			return;

		//! set emession color

		colorf emession (pkMaterial->emession);
		float emessionf[4] = {emession.getRed(), emession.getGreen(), emession.getBlue(), emession.getAlpha() };
		glMaterialfv (GL_FRONT, GL_EMISSION, emessionf);

		//! set ambient color

		colorf ambient (pkMaterial->ambient);
		float ambientf[4] = {ambient.getRed(), ambient.getGreen(), ambient.getBlue(), ambient.getAlpha() };
		glMaterialfv (GL_FRONT, GL_AMBIENT, ambientf);

		//! set diffuse color

		colorf diffuse (pkMaterial->diffuse);
		float diffusef[4] = {diffuse.getRed(), diffuse.getGreen(), diffuse.getBlue(), diffuse.getAlpha() };
		glMaterialfv (GL_FRONT, GL_DIFFUSE, diffusef);

		//! set specular color

		colorf specular (pkMaterial->specular);
		float specularf[4] = {specular.getRed(), specular.getGreen(), specular.getBlue(), specular.getAlpha() };
		glMaterialfv (GL_FRONT, GL_SPECULAR, specularf);

		//! set shininess

		glMaterialf (GL_FRONT, GL_SHININESS, pkMaterial->shininess);

		//! set blending
#if 0
		int blendSrc=GL_ZERO;
		int blendDst=GL_ZERO;

		switch (pkMaterial->blend.getSrc()) {

			case SRC_ONE: blendSrc=GL_ONE;break;
			case SRC_COLOR:	blendSrc=GL_SRC_COLOR;break;
			case SRC_ONE_MINUS_SRC_COLOR: blendSrc=GL_ONE_MINUS_SRC_COLOR;break;
			case SRC_ALPHA: blendSrc=GL_SRC_ALPHA;break;
			case SRC_ONE_MINUS_SRC_ALPHA: blendSrc=GL_ONE_MINUS_SRC_ALPHA;break;
			case SRC_ALPHA_SATURATE: blendSrc=GL_SRC_ALPHA_SATURATE;break;
		};

		switch (pkMaterial->blend.getSrc()) {

			case DST_ONE: blendSrc=GL_ONE;break;
			case DST_COLOR:	blendSrc=GL_DST_COLOR;break;
			case DST_ONE_MINUS_DST_COLOR: blendSrc=GL_ONE_MINUS_DST_COLOR;break;
			case DST_ALPHA: blendSrc=GL_DST_ALPHA;break;
			case DST_ONE_MINUS_DST_ALPHA: blendSrc=GL_ONE_MINUS_DST_ALPHA;break;
		};

		glBlendFunc (blendSrc,blendDst);
#endif
		//! set textures

		for (int i = 0; i < pkMaterial->getTextureCount (); i++) {

			texture* pkTexture = pkMaterial->getTexture (i);

			if ( pkTexture )
				setTexture ( pkTexture );
		}

		//! Gouraud Shading enable

		if ( (updateStates) || (pkMaterial->gouraudShading != pkCurrentMaterial->gouraudShading)) {

			if (pkMaterial->gouraudShading==true)
				glShadeModel(GL_SMOOTH);
			else
				glShadeModel(GL_FLAT);
		}

		//! lighting enable

		if ( (updateStates) || (pkMaterial->lighting != pkCurrentMaterial->lighting) ) {

			if (pkMaterial->lighting==true)	//true but still skipped !???
				glEnable (GL_LIGHTING);
			else
				glDisable (GL_LIGHTING);
		}

		//! Z buffer enable

		if ( (updateStates) || ( pkMaterial->zBuffer != pkCurrentMaterial->zBuffer) ) {

			if (pkMaterial->zBuffer==true) {
				glEnable (GL_DEPTH_TEST);
			}else{
				glDisable (GL_DEPTH_TEST);
			}
		}

		//! z buffer write

		if ( (updateStates) || (pkMaterial->zWrite != pkCurrentMaterial->zWrite) ) {

			if (pkMaterial->zWrite==true)
				glDepthMask (GL_TRUE);
			else
				glDepthMask(GL_FALSE);
		}

		//! fog

//		if (gFog==true) {
			if ( (updateStates) || (pkMaterial->fog != pkCurrentMaterial->fog) ) {
				if (pkMaterial->fog==true)
					glEnable (GL_FOG);
				else
					glDisable (GL_FOG);
			}
//		}

		//! if we need to cull, enable it

		if (pkMaterial->backFaceCull || pkMaterial->frontFaceCull) {

			if (glIsEnabled (GL_CULL_FACE))
				glEnable (GL_CULL_FACE);
		}

		//! set front or back face culling

		if (pkMaterial->backFaceCull && pkMaterial->frontFaceCull) {

			glCullFace (GL_FRONT_AND_BACK);
		}
		else
		if (pkMaterial->backFaceCull)
			glCullFace (GL_BACK);
		else
		if (pkMaterial->frontFaceCull)
			glCullFace (GL_FRONT);
		else
			glDisable (GL_CULL_FACE);

		//! disable specular colors if no shininess is set

		if (pkMaterial->shininess != 0.0f)
			glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		else
			glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

		//! texture filtering mode

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			(pkMaterial->bilinearFilter || pkMaterial->trlinearFilter) ? GL_LINEAR : GL_NEAREST);

		if (pkMaterial->texture1 && pkMaterial->texture1->hasMipmaps() ) {

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				(pkMaterial->trlinearFilter) ? GL_LINEAR_MIPMAP_LINEAR
						: (pkMaterial->bilinearFilter)
						? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST );
		}
		else
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				(pkMaterial->bilinearFilter || pkMaterial->trlinearFilter) ? GL_LINEAR : GL_NEAREST);

//		if (AnisotropyExtension)
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
				pkMaterial->anisotropicFilter ? maxAnisotropyExt : 1.0f );


//		glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);

		//! set current material

		pkCurrentMaterial = pkMaterial;
	}
}

/**
*	Creates a system-defined font
*/
font* videoDriverOGL::createSystemFont (std::string fontname,
		int charWidth, int charHeight, int weight, CHAR_SET set) {

	font* pkFont = 0;

//#ifdef MERCURY_WINDOWS
#ifdef _WIN32

	//! create WGL compatable font
//	pkFont = new fontWGL;

	pkFont = 0;

	if (pkFont)
		pkFont->createFont (fontname, charWidth, charHeight, weight, set);

#endif

	return pkFont;
}

/**
*	Free's a system font
*/
void videoDriverOGL::freeSystemFont (font* pkFont) {

	if (pkFont) {

		delete pkFont;
		pkFont=0;
	}
}

/**
*	Loads extension
*/
void* videoDriverOGL::getExtension (std::string name) {

#ifdef _WIN32
	return wglGetProcAddress (name.c_str());
#endif
	return 0;
}

/**
*	Test if extension is supported
*/
bool videoDriverOGL::isExtension (std::string name) {

	const GLubyte* pExt = glGetString (GL_EXTENSIONS);
	return strstr((const char*)pExt, name.c_str()) == 0 ? false : true;
}

/**
*	Allocates shader program
*/
shaderProgram* videoDriverOGL::addShader () {

	return new shaderProgramOGL();
}

#endif



#if 0
/**
*	Creates system font
*/
void fontWGL::createFont (std::string fontName,
		int kcharWidth, int kcharHeight, int weight, CHAR_SET set) {

	//! storage for 96 characters
	baseDisplayList = glGenLists (96);

	//! create the system font
	newFont = CreateFont (
		kcharHeight, //height of font
		kcharWidth, //width
		0,//escapement
		0,//oriantation,
		weight, //weight
		FALSE,//italic
		FALSE,//underline
		FALSE,//strikethrough
		ANSI_CHARSET, //charset
		OUT_TT_PRECIS,//output percision
		CLIP_DEFAULT_PRECIS,//clipping percision
		ANTIALIASED_QUALITY,//output quality
		FF_DONTCARE|DEFAULT_PITCH, //family and pitch
		fontName.c_str() //font name
		);

	//! get our window
	videoDriverOGL* pkVideo = (videoDriverOGL*)kernel::get()->getVideoDriver();
	HWND hwnd = FindWindow (0, pkVideo->getCaption().c_str());

	//! get DC to our window
	HDC hdc = GetDC (hwnd);

	//! select it into the DC
	oldFont = (HFONT)SelectObject (hdc, newFont);

	//! Bug in WGL here. wglUseFontBitmap may fail without error,
	//! so might need to try more then once

	//! builds 96 characters starting at character 32
	if (wglUseFontBitmaps (hdc,32,96,baseDisplayList)==FALSE) {

		if (wglUseFontBitmaps (hdc,32,96,baseDisplayList)==FALSE)
			std::cout << "*** fontWGL::createFont: wgl error: " << GetLastError () << std::endl;
	}

	//! selects old font
	SelectObject(hdc, oldFont);

	//! delete font-its in the DC now
	DeleteObject (newFont);

	//! set font properties
	charWidth  = kcharWidth;
	charHeight = kcharHeight;
	charWeight = weight;
	charSet    = CHARSET_ANSI;
	face       = fontName;
}

/**
*	Releases system font
*/
void fontWGL::destroyFont () {

	//! free display lists
	glDeleteLists(baseDisplayList, 96);
}

/**
*	Renders system font
*/
void fontWGL::render (std::string str, point2d<int> pos, color col ) {

	GLboolean isLight = glIsEnabled (GL_LIGHTING);
	GLboolean isTexture = glIsEnabled (GL_TEXTURE_2D);
	GLboolean isFog = glIsEnabled (GL_FOG);

	videoDriver* pkVideo = kernel::get()->getVideoDriver();

	int width=0,height=0;
	pkVideo->getResolution (&width, &height);

	signed int xPlus = -(width >> 1);
	float xFact = 1.0f / (width >> 1);

	signed int yPlus = height - (height >> 1);
	float yFact = 1.0f / (height >> 1);

	//! disable textures and lighting
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_LIGHTING);
	glDisable (GL_FOG);

	//! set color
	colorf c(col);
	glColor4f (c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());

	//! glRasterPos2f requires y on bottom not top, so adjust
	int realY = pos.y + charHeight;

	//! set position
	glRasterPos2f ( (pos.x + xPlus) * xFact, (yPlus - realY) * yFact);

	//! set base character to 0
	glPushAttrib(GL_LIST_BIT);
	glListBase (baseDisplayList - 32);

	//! renders display list
	glCallLists(str.length(), GL_UNSIGNED_BYTE, str.c_str() );
	glPopAttrib ();

	//! re-enable lighting
	if (isLight)
		glEnable (GL_LIGHTING);

	//! re-enable textures
	if (isTexture)
		glEnable (GL_TEXTURE_2D);

	//! re-enable fog
	if (isFog)
		glEnable (GL_FOG);

	//! set previous color
	glColor4f(1,1,1,1);
}

#endif
