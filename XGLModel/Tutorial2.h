#pragma once
#include "TutorialFactory.h"

namespace XGLModel {
	class Tutorial2 :public TutorialInterface
	{
	public:
		Tutorial2();
		virtual ~Tutorial2();
	public:

	protected:
		void createVertexBuffer();
		void drawByEnableClientState();
		void drawByEnableClientStateVBO();
		void drawByBeginEnd();
		void drawByCalllise();
		void drawByVBO();


		// Í¨¹ý TutorialInterface ¼Ì³Ð

		virtual void draw() override;

		virtual void initGL() override;

		virtual void initUniform() override;

		virtual void initShader() override;

	};
}



