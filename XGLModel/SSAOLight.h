#pragma once

#include "TutorialFactory.h"

namespace XGLModel {

	class IXMesh;
	class SSAOCore;
	class SSAOGeometry;
	class Blur;
	class XIOBuffer;

	class SSAOLight : public TutorialInterface
	{
	public:
		SSAOLight();
		~SSAOLight();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
		virtual void initCamera()override;

		void geometryPass();
		void corePass();
		void blurPass();
		void lightPass();

		GLint g_mv;

		GLint g_screensize,g_aomap, g_colormap;
		GLint g_mvp;
		IXMesh* m_Mesh1;
		SSAOGeometry* m_geometry;
		SSAOCore* m_core;
		Blur* m_blur;
		XIOBuffer* m_geo_buffer;
		XIOBuffer* m_core_buffer;
		XIOBuffer* m_blur_buffer;
		IXMesh* m_pQuad;
		Matrixf m_mv;
	};


}

