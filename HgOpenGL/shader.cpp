/*****************************************************

	shaderOGL.cpp
		OpenGL Shader support

*****************************************************/

#include "shader.h"
#include "glee.h"

using namespace hgg;

namespace hgogl {

/**
* Initialize shader program
*/
HgShaderProgramOGL::HgShaderProgramOGL ()
:	enabled(false) {
	programId = glCreateProgramObjectARB ();
}

/**
* Release resources
*/
HgShaderProgramOGL::~HgShaderProgramOGL() {
	glDeleteObjectARB (programId);
}

/**
* Enables and disables program
*/
void HgShaderProgramOGL::enable (bool b) {
	if (b)
		glUseProgramObjectARB (programId);
	else
		glUseProgramObjectARB (0);
	enabled=b;
}

/**
* Creates shader
*/
HgShader HgShaderProgramOGL::addShader (HgShaderType type) {

	unsigned int glType = 0;
	HgShader pkShader;

	if (type == HG_SHADER_VERTEX_PROGRAM)
		glType = GL_VERTEX_SHADER;
	else
	if (type == HG_SHADER_FRAGMENT_PROGRAM)
		glType = GL_FRAGMENT_SHADER;
	else {
		pkShader.type = HG_SHADER_UNKNOWN_PROGRAM;
		return pkShader;
	}
	pkShader.type=type;
	pkShader.id = glCreateShaderObjectARB (glType);
	glAttachShader (programId, pkShader.id);
	return pkShader;
}

/**
* Removes shader
*/
void HgShaderProgramOGL::deleteShader (HgShader s) {
	glDetachShader (programId, s.id);
	glDeleteObjectARB (s.id);
}

/**
* Returns compiled messages
*/
std::string HgShaderProgramOGL::getCompileMessage (HgShader s) {
	char log[100]={0};
	GLsizei len=0;
	glGetShaderInfoLog (s.id, 100, &len, log);
	return std::string (log);
}

/**
* Returns variable id
*/
unsigned int HgShaderProgramOGL::getVariable (std::string var) {
	if (!programId)
		return -1;
	return glGetUniformLocation (programId, var.c_str ());
}

/**
* Compiles shader file
*/
void HgShaderProgramOGL::compile (HgShader s, std::string f) {
	const char* p = f.c_str();
	glShaderSource ( s.id, 1, (const char**) &p, 0);
	s.compiled = true;
	glCompileShader (s.id);
}

#if 0
/**
* Compiles from source file
*/
void HgShaderProgramOGL::compile (HgShader s, file* pkFile) {

	if (pkFile) {

		unsigned int len=pkFile->getSize()-1;
		unsigned int len2=len;

		// read entire file
		char* buf = new char [len];
		pkFile->read(buf,len);

		// go to beginning of last line
		while (buf[len] != '\r' && buf[len] != '\n')
			len--;

		// get rid of leading garbage chars
		while (len++ < len2)
			if (buf[len] < 32 && buf[len]!='\t') {

				buf[len-1]=0; //null terminate here
				break;
			}

		// compile
		compile (s, buf);

		// release memory
		delete [] buf;
		buf=0;
	}
}
#endif

/**
* Links program
*/
void HgShaderProgramOGL::link () {
	glLinkProgram (programId);
}

}
