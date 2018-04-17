#pragma once
#include "TutorialFactory.h"
namespace XGLModel {
	class Tutorial4 : public TutorialInterface
	{
	public:
		Tutorial4();
		virtual ~Tutorial4();

		// 通过 TutorialInterface 继承
		virtual void init() override;
		virtual void draw() override;

		// 通过 TutorialInterface 继承
		virtual void initShader() override;
	};
}


