#pragma once
#include "XGLModel.h"
#include "ModelGL.h"

namespace XGLModel {
	class XGLMODEL_API TutorialInterface :public ModelGL
	{
	public:
		TutorialInterface();
		virtual ~TutorialInterface();
	public:
		virtual void init() override = 0;
		virtual void draw() override = 0;
	};
}


