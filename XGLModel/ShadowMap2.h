#pragma once

#include "TutorialFactory.h"
#include "XGLModel.h"
#include "XLight.h"
#include "XLightShader.h"
namespace XGLModel {
	class IXMesh;
	class ShadowMap2 : public TutorialInterface
	{
	public:
		ShadowMap2();
		virtual ~ShadowMap2();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void draw() override;
		virtual void initGL() override;
		virtual void initUniform() override;
		virtual void initCamera() override;
	private:
		IXMesh * m_pMesh;
		IXMesh* m_pQuad;
		GLint g_mv;
		GLint g_perspective;
		GLint g_sampler;
		GLuint m_fbo;
		GLuint m_texShadow;
		SpotLighter spotlight;
		XLightShader lightShader;
		
	};


}

