#pragma once
#include "TutorialFactory.h"

namespace XGLModel {

	class XPatchMesh;
	class XTexture;
	class PNTessellation : public TutorialInterface
	{
	public:
		PNTessellation();
		~PNTessellation();

		// ͨ�� TutorialInterface �̳�
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
		GLint g_tesslevel;

	};


}

