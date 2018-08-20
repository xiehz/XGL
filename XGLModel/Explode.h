#pragma once

#include "TutorialFactory.h"

namespace XGLModel {

	class IXMesh;
	class Explode : public TutorialInterface
	{
	public:
		Explode();
		~Explode();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		void setExploder(float ex);
		virtual void draw() override;
		virtual void initUniform() override;
	public:
		GLuint g_world;
		GLuint g_vp;
		GLuint g_exploder;
		IXMesh* m_layer;
	};


}

