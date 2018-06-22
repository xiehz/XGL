#pragma once

#include "TutorialFactory.h"
#include "SpotLight.h"
#include "XLightShader.h"
namespace XGLModel {

	class XTexture;
	class IXMesh;
	class AxesShape;
	class NormalMap :public TutorialInterface
	{
	public:
		NormalMap();
		~NormalMap();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
		virtual void initCamera() override;
	private:
		GLuint g_mv;
		GLuint g_pers;
		GLuint g_sampler2d;
		GLuint g_NormalMap;

		XTexture* m_pMaterial;
		XTexture* m_pNormalMap;
		IXMesh* m_pBox;

		AxesShape* m_pAxes;
		SpotLighter spotlight;
		XLightShader lightShader;
	};



}
