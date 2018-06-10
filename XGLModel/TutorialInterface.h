#pragma once
#include <string>
#include "XGLUtil.h"
#include "XGLModel.h"
#include "ModelGL.h"

namespace XGLModel {
	class XGLMODEL_API TutorialInterface :public ModelGL
	{
	public:
		TutorialInterface();
		virtual ~TutorialInterface();
		TutorialInterface* setName(const std::string& name) { 
			this->name = name;
			return this;
		}
	public:
		virtual void draw() override = 0;
		virtual void initGL() = 0 ;
	protected:
		virtual void initShader() override;
		virtual void initUniform() = 0;
		void readShader(const char* filename, std::string& source);
		void addShader(GLuint shaderProgram,GLuint shader, const std::string& source);
		void linkProgram();
		void validateProgram();

		GLuint program;
		GLuint vs, gs,ts,fs;
		GLuint vbo;
		GLuint ebo;
		std::string vsSource;
		std::string gsSource;
		std::string tsSource;
		std::string fsSource;
		std::string name;
	};

	inline void TutorialInterface::initShader()
	{
		program = glCreateProgram();
		if (!program)
		{
			XGLERROR("error create program");
		}

		vs = glCreateShader(GL_VERTEX_SHADER);
		if (!vs)
		{
			XGLERROR("error create shader");
			return;
		}

		std::string directory = "..\\..\\XGLModel\\";
		std::string vsname =directory + this->name + ".vert";
		readShader(vsname.c_str() , vsSource);
		addShader(program, vs, vsSource);
		postViewMsg(1, vsSource);

		fs = glCreateShader(GL_FRAGMENT_SHADER);
		if (!fs)
		{
			XGLERROR("error create shader");
			return;
		}

		std::string fsname = directory + this->name + ".frag";
		readShader(fsname.c_str(), fsSource);
		addShader(program, fs, fsSource);
		postViewMsg(4, fsSource);

		linkProgram();

		initUniform();

		validateProgram();

		glUseProgram(program);
	}

	inline void TutorialInterface::readShader(const char * filename,std::string& source)
	{
		if (!readShaderFile(filename, source))
		{
			XGLERROR(filename);
		}
	}

	inline void TutorialInterface::addShader(GLuint program, GLuint shader, const std::string& content)
	{
		const GLchar* p[1];
		p[0] = content.c_str();
		GLint length[1];
		length[0] = (GLint)strnlen_s(p[0], 65355);
		glShaderSource(shader, 1, p, length);
		glCompileShader(shader);

		GLint success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar errorInfo[1024];
			glGetShaderInfoLog(shader, sizeof(errorInfo), 0, errorInfo);
			XGLERROR(errorInfo);
			return;
		}

		glAttachShader(program, shader);
	}
	inline void TutorialInterface::linkProgram()
	{
		GLint success = 0;
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar errorLog[1024];

			glGetProgramInfoLog(program, sizeof(errorLog), nullptr, errorLog);
			XGLERROR(errorLog);
		}

	}
	inline void TutorialInterface::validateProgram()
	{
		GLint success = 0;

		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
		if (!success)
		{
			GLchar errorLog[1024];

			glGetProgramInfoLog(program, sizeof(errorLog), nullptr, errorLog);
			XGLERROR(errorLog);
		}
	}
}


