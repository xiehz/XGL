#pragma once

#include "TutorialFactory.h"
#include "xgl/Matrixf"
#include "IXMesh.h"
#include "XCubemapTexture.h"
#include "SphereShape.h"
namespace XGLModel {
	class SkyCube :public TutorialInterface
	{
	public:
		SkyCube();
		~SkyCube();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
	public:
		void loadModel();
		void render();
		void setCamera(const XGL::Matrixf& c);
		void setModel(const XGL::Matrixf& m);
	private:
		XGL::Matrixf model;
		GLint g_mv;
		GLint g_pers;
		GLint g_cubeSampler;
		IXMesh* m_pCube;
		SphereShape* m_pSphere;
		XCubemapTexture* m_pCubeTex;
	};

}

