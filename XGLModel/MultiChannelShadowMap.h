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
	class CubeShadowMap;
	struct TagPointLightLocation;
	class MultiChannelShadowMap : public TutorialInterface
	{
	public:
		MultiChannelShadowMap();
		virtual ~MultiChannelShadowMap();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void draw() override;
		virtual void initGL() override;
		virtual void initUniform() override;
		void activeLights();
		void updateLight(const TagPointLight & pl, const unsigned int index, float materialIntensity, float shineness);
		virtual void initCamera() override;
		void setupPers();
	private:
		void renderShadow();
		void render();
	private:
		static int N;
		IXMesh * m_pMesh;
		IXMesh* m_pQuad;
		CubeShadowMap* m_cubeShadowShader;

		GLint g_world;
		GLint g_vp;
		GLint g_eye_world;
		GLint g_sampler;
		GLint g_shadowmap;
		TagPointLight* m_PointLight;
		TagPointLightLocation* m_pointLocation;
		GLint g_shineness;
		GLint g_materialIntensity;
		GLint  g_N;
		XCubeShadowBuffer* m_pCubeBuffer;
		XTexture* m_pTexture;
		SphereShape* m_Sphere;
	};
	struct TagPointLightLocation
	{
		GLint Color;
		GLint AmbientIntensity;
		GLint DiffuseIntensity;
		GLint Eposition;
		struct TagAttenuationLocation
		{
			GLint Constant;
			GLint Linear;
			GLint Exp;
		}Attenuation;
	};


}

