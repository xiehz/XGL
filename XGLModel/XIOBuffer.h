#pragma once
#include "GL/glew.h"
namespace XGLModel {

	class XIOBuffer
	{
	public:
		XIOBuffer();
		~XIOBuffer();
	public:
		bool init(unsigned int width, unsigned int height, bool withDepth, GLenum internaltype);
		void bindForWriting();
		void bindForReading(GLenum texunit);
		void bindForBlit();

	private:
		GLuint m_fbo;
		GLuint m_texture;
		GLuint m_depth;
		GLenum m_internaleType;
	};


}

