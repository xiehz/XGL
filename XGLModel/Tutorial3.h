#pragma once
#include "TutorialFactory.h"

namespace XGLModel {

	class Tutorial3: TutorialInterface
	{
	public:
		Tutorial3();
		virtual ~Tutorial3();

		// 通过 TutorialInterface 继承
		virtual void init() override;
		virtual void draw() override;
	protected:
		void drawFixedPipeline();


		// 通过 TutorialInterface 继承
		virtual void initShader() override;


	};
}


