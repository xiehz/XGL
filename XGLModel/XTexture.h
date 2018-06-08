#pragma once

#include "GL/glew.h"
#include "Magick++.h"
#include "XGLModel.h"

namespace XGLModel {
	class XTexture
	{
	public:
		XTexture(GLenum target, const std::string& imageName);
		~XTexture();

	public:
		bool Load();
		void Bind(GLenum texUnit);
	private:
		GLuint m_texObj;
		GLuint m_texTarget;
		std::string m_texFile;
	};
}

