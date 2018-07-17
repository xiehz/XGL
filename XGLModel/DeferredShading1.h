#pragma once
#include "XLight.h"
#include "TutorialFactory.h"

namespace XGLModel {

	class DirLightShader;
	class PointLightShader;
	class IXMesh;
	class XGBuffer;
	class DeferredShading1 : public TutorialInterface
	{
	public:
		DeferredShading1();
		~DeferredShading1();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
		virtual void initCamera()override;

		GLint g_world;
		GLint g_vp;

		IXMesh* m_Mesh1;
		XGBuffer* m_Gbuffer;

		IXMesh* m_LightQuad;
		IXMesh* m_LightSphere;

		TagDirectionLight m_DirectionLight;
		TagPointLight m_PointLight[2];

		DirLightShader* m_ShaderDirectionLight;
		PointLightShader* m_ShaderPointLight;
	private:
		void initLight();
	};


}

