#pragma once
#include "TutorialFactory.h"

namespace XGLModel {

	class Tutorial3: TutorialInterface
	{
	public:
		Tutorial3();
		virtual ~Tutorial3();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void init() override;
		virtual void draw() override;
	protected:
		void drawFixedPipeline();
	};
}


