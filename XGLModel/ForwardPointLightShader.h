#pragma once
#include "GL\glew.h"
#include "xgl\Vec3f"
#include "XGLModel.h"
#include "XLight.h"
#include "TutorialInterface.h"

namespace XGLModel {

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
	class ForwardPointLightShader : public TutorialInterface
	{
	public:
		ForwardPointLightShader(int n);
		~ForwardPointLightShader();
	public:
		// 通过 TutorialInterface 继承
		virtual void initUniform() override;
		void activeLights();
		void updateLight(const TagPointLight& pl, const unsigned int index, float materialIntensity, float shineness);
		void updateMT(const XGL::Matrixf& mv, const XGL::Matrixf& pers);
	private:
		const unsigned int N;

		TagPointLightLocation* m_pointLocation;
		GLint g_shineness;
		GLint g_materialIntensity;
		GLint  g_mv;
		GLint  g_normal;
		GLint  g_mvp;
		GLint  g_N;

		// 通过 TutorialInterface 继承
		virtual void initGL() override;
		virtual void draw() override;
	};


}

