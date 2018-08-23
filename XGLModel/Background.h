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

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;		
		void render() ;

		GLint g_sampler;
		XTexture* m_pTex;

		IXMesh* m_LightQuad;
	
	};


}

