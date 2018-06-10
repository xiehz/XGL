#pragma once
#include "TutorialFactory.h"
#include "XGLModel.h"

namespace XGLModel {

	class IXMesh;
	class ShadowMap : public TutorialInterface
	{
	public:
		ShadowMap();
		~ShadowMap();
	public:
		// ͨ�� TutorialInterface �̳�
		virtual void initGL() override;

		virtual void draw() override;

		virtual void initUniform() override;
	protected:
		IXMesh * m_pMesh;
		GLint g_mv;
		GLint g_perspective;
		GLint g_sampler;
	};



}
