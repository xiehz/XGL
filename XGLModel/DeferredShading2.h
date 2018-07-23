#pragma once
#include "XLight.h"
#include "TutorialFactory.h"

namespace XGLModel {

	class DirLightShader;
	class PointLightShader;
	class IXMesh;
	class XGBuffer;
	class DSSphere;
	class DeferredShading2 : public TutorialInterface
	{
	public:
		DeferredShading2();
		~DeferredShading2();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
		virtual void initCamera()override;

		void dsGeometryPass();
		void dsStencilPass(unsigned int index);
		void dsDirectionLightPass();
		void dsPointLightPass(unsigned int index);
		void dsFinalPass();

		GLint g_mv;
		GLint g_mvp;

		IXMesh* m_Mesh1;
		XGBuffer* m_Gbuffer;

		IXMesh* m_LightQuad;

		TagDirectionLight m_DirectionLight;
		TagPointLight m_PointLight[2];

		DirLightShader* m_ShaderDirectionLight;
		PointLightShader* m_ShaderPointLight;
		DSSphere* m_dsSphere[2];
	private:
		void initLight();
	};


}

