#pragma once

#include "TutorialFactory.h"
#include "XGLModel.h"

namespace XGLModel {

	class AxesShape : public TutorialInterface
	{
	public:
		AxesShape();
		virtual ~AxesShape();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void draw() override;
		virtual void initGL() override;
	public:
		void loadModel();
		void setCamera(const Matrixf& m);
		void setModel(const Matrixf& m);
		void render();
		virtual void initUniform() override;
	private:
		GLint g_mv;
		GLint g_pers;
		Matrixf model;
	};



}
