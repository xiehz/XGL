#pragma once
#include "XLight.h"
#include "TutorialFactory.h"

namespace XGLModel {

	class IXMesh;

	class DSSphere : public TutorialInterface
	{
	public:
		DSSphere();
		~DSSphere();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
		void setCamera(const XGL::Matrixf & camera);
		void setModelByPointLight(const TagPointLight& pointlight);

		IXMesh* m_LightSphere;
		GLint g_mv;
		GLint g_mvp;
		XGL::Matrixf m_WorldMT;
	private:
		float CalcPointLightBSphere(const XGLModel::TagPointLight & Light);

	};


}

