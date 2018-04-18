#pragma once
#include "TutorialFactory.h"

namespace XGLModel {

	class Tutorial3: TutorialInterface
	{
	public:
		Tutorial3();
		virtual ~Tutorial3();

		// ͨ�� TutorialInterface �̳�
		virtual void init() override;
		virtual void draw() override;
	protected:
		void drawFixedPipeline();


		// ͨ�� TutorialInterface �̳�
		virtual void initShader() override;


		// ͨ�� TutorialInterface �̳�
		virtual int onMessage(int id, std::string & msg) override;


	};
}


