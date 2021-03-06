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
	class DepthImage;
	class PcfShadowMap : public TutorialInterface
	{
	public:
		PcfShadowMap();
		virtual ~PcfShadowMap();

		// ͨ�� TutorialInterface �̳�
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
		GLint g_ScreenSize;
		GLuint m_fbo;
		GLuint m_texShadow;
		SpotLighter spotlight;
		XLightShader lightShader;
		XTexture* bkg;
		AxesShape* m_Axes;
		SphereShape* m_Sphere;
		DepthImage* m_DepthImage;
	};


}

