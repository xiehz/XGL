#pragma once

#include "TutorialFactory.h"
#include "xgl/Matrixf"
#include "IXMesh.h"
#include "XCubemapTexture.h"
#include "SphereShape.h"
namespace XGLModel {
	class CubeShape :public TutorialInterface
	{
	public:
		CubeShape();
		~CubeShape();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
	public:
		void loadModel();
		void render();
		void setCamera(const XGL::Matrixf& c);
		void setModel(const XGL::Matrixf& m);

		XGL::Matrixf model;
		GLint g_world;
		GLint g_vp;
		GLint g_cubeSampler;
		IXMesh* m_pCube;
		XCubemapTexture* m_pCubeTex;
	};

}

