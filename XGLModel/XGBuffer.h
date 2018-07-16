#pragma once
#include "GL/glew.h"
namespace XGLModel {

	class XGBuffer
	{
	public:
		enum GBUFFER_TEXTURE_TYPE
		{
			GBUFFER_TEXTURE_POSITION,
			GBUFFER_TEXTURE_DIFFUSE,
			GBUFFER_TEXTURE_NORMAL,
			GBUFFER_TEXTURE_TEXCOORD
		};
	public:
		XGBuffer();
		~XGBuffer();
	public:
		bool init(unsigned int width, unsigned int height);
		void bindForWriting();
		void bindForReading();
		void setReadBuffer(GBUFFER_TEXTURE_TYPE gbuffer_texture);
	private:
		GLuint m_fbo;
		GLuint m_textures[4];
		GLuint m_depthTex;
	};


}

