#pragma once
#include "TutorialFactory.h"
#include "IXMesh.h"

namespace XGLModel {

	class Picking;
	class SphereShape;

	class PickingDemo : public TutorialInterface
	{
	public:
		PickingDemo();
		~PickingDemo();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		void drawPicking();
		void drawScene(const XGLModel::PixelInfo& pixel);
		virtual void initUniform() override;
	private:
		GLuint g_mv;
		GLuint g_pers;
		GLuint g_sampler2d;
		IXMesh m_model;
		Picking* m_picking;
		SphereShape* m_hightlight;
	};


}

