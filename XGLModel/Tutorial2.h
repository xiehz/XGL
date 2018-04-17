#pragma once
#include "TutorialFactory.h"

namespace XGLModel {
	class Tutorial2 :public TutorialInterface
	{
	public:
		Tutorial2();
		virtual ~Tutorial2();
	public:


		// ͨ�� TutorialInterface �̳�
		virtual void init() override;

		virtual void draw() override;
	protected:
		void createVertexBuffer();
		void drawByEnableClientState();
		void drawByEnableClientStateVBO();
		void drawByBeginEnd();
		void drawByCalllise();
		void drawByVBO();
	private:


		// ͨ�� TutorialInterface �̳�
		virtual void initShader() override;

	};
}



