#pragma once
#include "XLight.h"
#include "TutorialFactory.h"

namespace XGLModel {


	class IXMesh;
	class XTexture;
	class Background : public TutorialInterface
	{
	public:
		Background();
		~Background();

		// ͨ�� TutorialInterface �̳�
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;		
		void render() ;

		GLint g_sampler;
		XTexture* m_pTex;

		IXMesh* m_LightQuad;
	
	};


}

