#pragma once

#include "XGLModel.h"
#include "TutorialFactory.h"

namespace XGLModel {
	class AmbientLight : public TutorialInterface
	{
	public:
		AmbientLight();
		virtual ~AmbientLight();

		// ͨ�� TutorialInterface �̳�
		virtual void init() override;
		virtual void draw() override;
		virtual void initShader() override;
	};
}
