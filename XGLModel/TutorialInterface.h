#pragma once
#include "XGLUtil.h"
#include "XGLModel.h"
#include "ModelGL.h"

namespace XGLModel {
	class XGLMODEL_API TutorialInterface :public ModelGL
	{
	public:
		TutorialInterface();
		virtual ~TutorialInterface();
	public:
		virtual void init() override = 0;
		virtual void draw() override = 0;
		virtual int onMessage(int id, std::string& msg) = 0;

	protected:
		virtual void initShader() = 0;
		void readShader(const char* filename, std::string& source);
		void addShader(GLuint shaderProgram,GLuint shader, const std::string& source);
		void linkProgram();
		void validateProgram();

		GLuint program;
		GLuint vs, fs;
		GLuint vbo;
		GLuint ebo;
		std::string vsSource;
		std::string gsSource;
		std::string tsSource;
		std::string fsSource;
	};


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
		length[0] = (GLint)strnlen_s(p[0], 1024);
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


