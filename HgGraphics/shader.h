/*****************************************************

	shader.h
		Material Shader

*****************************************************/

#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <string>

namespace hgg {

/**
* Shader program type
*/
enum HgShaderType {
	HG_SHADER_VERTEX_PROGRAM,
	HG_SHADER_FRAGMENT_PROGRAM,
	HG_SHADER_UNKNOWN_PROGRAM
};

/**
* Shader object
*/
class HgShader {

public:

	std::string name;
	unsigned int id;
	bool compiled;
	HgShaderType type;

	HgShader ()
		: id(0), compiled(false), type(HG_SHADER_UNKNOWN_PROGRAM) { }

	inline HgShaderType getType ()   {return type;}
	inline bool         isCompiled() {return compiled;}
	inline std::string  getName ()   {return name;}
	inline unsigned int getId ()     {return id;}
};

/**
* Shader program
*/
class HgShaderProgram {

protected:

	unsigned int programId;
	bool enabled;

public:

	HgShaderProgram () : programId (0), enabled(false) {}

	inline bool          isEnabled () {return enabled;}
	virtual void         enable (bool) = 0;
	virtual HgShader     addShader (HgShaderType) = 0;
	virtual void         deleteShader (HgShader) = 0;
	virtual unsigned int getVariable (std::string var) = 0;
	virtual void         compile (HgShader, std::string) = 0;
	virtual std::string  getCompileMessage (HgShader) = 0;
	virtual void         link () = 0;
};

} // namespace hgg

#endif
