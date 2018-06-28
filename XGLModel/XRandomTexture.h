#pragma once

#include "GL/glew.h"

namespace XGLModel {
	class XRandomTexture
	{
	public:
		XRandomTexture();
		~XRandomTexture();

	public:
		bool Init(unsigned int Size);
		void Bind(GLenum texUnit);
	private:
		GLuint m_texObj;
	};
}
