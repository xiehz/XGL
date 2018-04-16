#pragma once
#include "TutorialFactory.h"

namespace XGLModel {
	class Tutorial2 :public TutorialInterface
	{
	public:
		Tutorial2();
		virtual ~Tutorial2();
	public:


		// Í¨¹ý TutorialInterface ¼Ì³Ð
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
		unsigned int vbo;

	};
}



