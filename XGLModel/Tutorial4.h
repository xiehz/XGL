#pragma once
#include "TutorialFactory.h"
namespace XGLModel {
	class Tutorial4 : public TutorialInterface
	{
	public:
		Tutorial4();
		virtual ~Tutorial4();

		// ͨ�� TutorialInterface �̳�

		virtual void draw() override;

		virtual void initGL() override;

		virtual void initUniform() override;

	};
}


