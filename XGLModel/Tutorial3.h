#pragma once
#include "TutorialFactory.h"

namespace XGLModel {

	class Tutorial3:public TutorialInterface
	{
	public:
		Tutorial3();
		virtual ~Tutorial3();

	protected:
		void drawFixedPipeline();



		// ͨ�� TutorialInterface �̳�

		virtual void draw() override;

		virtual void initGL() override;

		virtual void initUniform() override;

		virtual void initShader() override;

	};
}


