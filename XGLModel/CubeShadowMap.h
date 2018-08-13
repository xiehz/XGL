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

	class CubeShadowMap : public TutorialInterface
	{
	public:
		CubeShadowMap();
		virtual ~CubeShadowMap();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void draw() override;
		virtual void initGL() override;
		virtual void initUniform() override;

		void setupPers(XGL::Vec3f eye, int face);

		void setWorld(const XGL::Matrixf& world);
		GLint g_world;
		GLint g_vp;
		GLint g_eyeworld;
	};

	struct XCameraDirection {
		GLenum cubeMapFace;
		XGL::Vec3f target;
		XGL::Vec3f up;
	};



}

