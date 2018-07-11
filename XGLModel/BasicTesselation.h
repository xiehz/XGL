#pragma once
#include "TutorialFactory.h"

namespace XGLModel {

	class XPatchMesh;
	class XTexture;
	class BasicTesselation : public TutorialInterface
	{
	public:
		BasicTesselation();
		~BasicTesselation();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
	private:
		XPatchMesh * m_patchMesh;
		XTexture* m_heightMap;
		GLint g_heightmap;
		GLint g_world;
		GLint g_view;
		GLint g_pers;
		GLint g_eye;

	};


}

