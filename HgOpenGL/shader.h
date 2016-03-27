/*****************************************************

	shaderOGL.h
		OpenGL Shader support

*****************************************************/

#ifndef SHADER_OGL_H_INCLUDED
#define SHADER_OGL_H_INCLUDED

#include <hggraphics/shader.h>

namespace hgogl {

/**
*	OpenGL Shader Program
*/
class HgShaderProgramOGL : public hgg::HgShaderProgram {

	bool enabled;

public:

	HgShaderProgramOGL();

	virtual ~HgShaderProgramOGL ();

	virtual void enable (bool);

	virtual hgg::HgShader addShader (hgg::HgShaderType);

	virtual void deleteShader (hgg::HgShader);

	virtual unsigned int getVariable (std::string var);

	virtual void compile (hgg::HgShader, std::string);

	virtual std::string getCompileMessage (hgg::HgShader);

	virtual void link ();
};

}

#endif
