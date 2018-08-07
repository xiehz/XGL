#pragma once
#include <GL/glew.h>
namespace XGLModel {

	class XCubeShadowBuffer
	{
	public:
		XCubeShadowBuffer();
		~XCubeShadowBuffer();

		bool init(unsigned int width, unsigned int height);
		void bindForWriting(GLenum glFace);
		void bindForReading(GLenum texUnit);

	public:
		GLuint m_fbo;
		GLuint m_shadowMap;
		GLuint m_depth;
	};


}

