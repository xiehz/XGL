#pragma once
#include "TutorialFactory.h"
namespace XGLModel {
	class Tutorial4 : public TutorialInterface
	{
	public:
		Tutorial4();
		virtual ~Tutorial4();

		// ͨ�� TutorialInterface �̳�
		virtual void init() override;
		virtual void draw() override;

		// ͨ�� TutorialInterface �̳�
		virtual void initShader() override;
	};
}


