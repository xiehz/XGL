#pragma once

#include "TutorialFactory.h"

namespace XGLModel {

	class IXMesh;
	class XGBuffer;
	class DeferredShading0 : public TutorialInterface
	{
	public:
		DeferredShading0();
		~DeferredShading0();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
		virtual void initCamera()override;

		GLuint g_world;
		GLuint g_vp;
		IXMesh* m_Mesh1;
		XGBuffer* m_Gbuffer;
	};


}

