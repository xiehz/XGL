#pragma once
#include "XLight.h"
#include "TutorialFactory.h"

namespace XGLModel {
	class XAdjacencyMesh;
	class ForwardPointLightShader;
	class AxesShape;
	class EdgeDetection : public TutorialInterface
	{
	public:
		EdgeDetection();
		~EdgeDetection();

		// ͨ�� TutorialInterface �̳�
		virtual void initGL() override;
		Matrixf lightToWorld(XGL::Vec3f worldposition);
		virtual void draw() override;
		virtual void initUniform() override;
		virtual void initCamera() override;
	public:
		static unsigned int NLIGHT;
		GLuint g_world;
		GLuint g_mvp;
		GLuint g_lightPos;

		XGL::Vec3f* m_lightWorlds;
		TagPointLight* m_PointLight;
		ForwardPointLightShader * m_fpLightShader;
		XAdjacencyMesh* m_pBox;
		AxesShape* m_pAxesShader;
	};


}

