#pragma once

#include "TutorialFactory.h"
#include "XGLModel.h"
#include "XLight.h"
#include "XLightShader.h"
#include "XTexture.h"

namespace XGLModel {
	class IXMesh;
	class AxesShape;
	class SphereShape;
	class DepthImage;
	class XCubeShadowBuffer;

	class CubeShadowMap : public TutorialInterface
	{
	public:
		CubeShadowMap();
		virtual ~CubeShadowMap();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void draw() override;
		virtual void initGL() override;
		virtual void initUniform() override;

		void setupPers();

		void setWorld(XGL::Vec3f eye, int face, const XGL::Matrixf& world);
		GLint g_mv;
		GLint g_pers;
	};

	struct XCameraDirection {
		GLenum cubeMapFace;
		XGL::Vec3f target;
		XGL::Vec3f up;
	};



}

