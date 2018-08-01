#pragma once
#include "XLight.h"
#include "TutorialFactory.h"

namespace XGLModel {
	class XAdjacencyMesh;
	class ForwardPointLightShader;
	class AxesShape;
	class DSSphere;
	class StencilShadow : public TutorialInterface
	{
	public:
		StencilShadow();
		~StencilShadow();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		Matrixf lightToWorld(XGL::Vec3f worldposition);
		virtual void draw() override;
		virtual void initUniform() override;
		virtual void initCamera() override;
	public:
		void depthPass();
		void shadowStencilPass();
		void shadowedPass();
		void ambientPass();
	public:
		static unsigned int NLIGHT;
		GLint g_mvp;
		GLint g_lightPos;

		XGL::Vec3f* m_lightWorlds;
		TagPointLight* m_PointLight;
		ForwardPointLightShader * m_fpLightShader;
		XAdjacencyMesh* m_pBox;
		AxesShape* m_pAxesShader;
		DSSphere* m_pDepthShader;
		XAdjacencyMesh* m_pQuad;
	};


}

