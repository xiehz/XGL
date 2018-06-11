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
		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;

		virtual void draw() override;

		virtual void initUniform() override;

		virtual void initCamera()override;
	protected:
		IXMesh * m_pMesh;
		IXMesh* m_pQuad;
		GLint g_mv;
		GLint g_perspective;
		GLint g_sampler;
		GLuint m_fbo;
		GLuint m_texShadow;
	};



}
