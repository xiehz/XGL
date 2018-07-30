#pragma once
#include "XLight.h"
#include "TutorialFactory.h"

namespace XGLModel {
	class IXMesh;
	class ForwardPointLightShader;
	class AxesShape;
	class EdgeDetection : public TutorialInterface
	{
	public:
		EdgeDetection();
		~EdgeDetection();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		Matrixf lightToWorld(XGL::Vec3f worldposition);
		virtual void draw() override;
		virtual void initUniform() override;
		virtual void initCamera() override;
	public:
		static unsigned int NLIGHT;
		GLuint g_eye;
		GLuint g_pers;
		GLuint g_sampler2d;

		TagPointLight* m_PointLight;
		ForwardPointLightShader * m_fpLightShader;
		IXMesh* m_pBox;
		AxesShape* m_pAxesShader;
	};


}

