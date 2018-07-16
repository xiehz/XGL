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
		virtual void initGL() = 0;
		virtual void draw() override = 0;
		virtual void initShader() override;
		virtual void initUniform() = 0;
	protected:
		void readShader(const char* filename, std::string& source);
		void addShader(GLuint shaderProgram,GLuint shader, const std::string& source);
		void linkProgram();
		void validateProgram();
	public:
		GLuint program;
		GLuint vs, tcs, tes, gs,fs;
		GLuint vbo;
		GLuint ebo;
		std::string vsSource;
		std::string gsSource;
		std::string tcsSource;
		std::string tesSource;
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
		postViewMsg(0, vsSource);

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

		//gs
		std::string& gsname = directory + this->name + ".geom";
		
		if (readShaderFile(gsname.c_str(), gsSource)) {

			gs = glCreateShader(GL_GEOMETRY_SHADER);
			if (!gs)
			{
				XGLERROR("error create gs shader");
				return;
			}
			addShader(program, gs, gsSource);
			postViewMsg(3, gsSource);
		}
		//tcs
		std::string& tcsname = directory + this->name + ".tesc";
		if (readShaderFile(tcsname.c_str(), tcsSource)) {

			tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
			if (!tcs)
			{
				XGLERROR("error create tesc shader");
				return;
			}
			addShader(program, tcs, tcsSource);
			postViewMsg(1, tcsSource);
		}


		//tes
		std::string& tesname = directory + this->name + ".tese";
		if (readShaderFile(tesname.c_str(), tesSource)) {

			tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
			if (!tes)
			{
				XGLERROR("error create tese shader");
				return;
			}
			addShader(program, tes, tesSource);
			postViewMsg(2, tesSource);
		}

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


