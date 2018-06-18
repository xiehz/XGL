#pragma once

#include "TutorialFactory.h"
#include "XGLModel.h"
#include "XLight.h"
#include "XLightShader.h"
#include "XTexture.h"

namespace XGLModel {
	class IXMesh;
	class AxesShape;
	class SphereShape;
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
		void renderShadow();
		void render();
	private:
		IXMesh * m_pMesh;
		IXMesh* m_pQuad;
		GLint g_lv;
		GLint g_mv;
		GLint g_perspective;
		GLint g_sampler;
		GLint g_samplerShadow;
		GLuint m_fbo;
		GLuint m_texShadow;
		SpotLighter spotlight;
		XLightShader lightShader;
		XTexture* bkg;
		AxesShape* m_Axes;
		SphereShape* m_Sphere;
	};


}

