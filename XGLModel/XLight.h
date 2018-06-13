#pragma once
#include "XGLModel.h"
#include "xgl/Vec3f";

namespace XGLModel {
	typedef struct TagLight
	{
		TagLight() :Color(1.0f, 1.0f, 1.0f), AmbientIntensity(0.2), DiffuseIntensity(0.8)
		{

		}
		TagLight(const TagLight& tagLight) :Color(tagLight.Color), AmbientIntensity(tagLight.AmbientIntensity),
			DiffuseIntensity(tagLight.DiffuseIntensity)
		{
		}

		TagLight& operator = (const TagLight& ta)
		{
			if (&ta == this)
			{
				return *this;
			}
			else {
				Color = ta.Color;
				AmbientIntensity = ta.AmbientIntensity;
				DiffuseIntensity = ta.DiffuseIntensity;
				return *this;
			}
		}

		XGL::Vec3f Color;
		float AmbientIntensity;
		float DiffuseIntensity;
	}Lighter;


	typedef struct TagAttenuation
	{
		TagAttenuation() {
			Constant = 1.0f;
			Linear = 0.1f;
			Exp = 0.01f;
		}
		TagAttenuation(const TagAttenuation& ta)
		{
			Constant = ta.Constant;
			Linear = ta.Linear;
			Exp = ta.Exp;
		}
		TagAttenuation& operator = (const TagAttenuation& ta)
		{
			if (&ta == this)
			{
				return *this;
			}
			else {
				Constant = ta.Constant;
				Linear = ta.Linear;
				Exp = ta.Exp;
				return *this;
			}
		}

		float Constant;
		float Linear;
		float Exp;
	}Attenuationer;

	typedef struct TagSpotLight : public TagLight {
		XGL::Vec3f Eposition;
		XGL::Vec3f Direction;
		float Cutoff;
		TagAttenuation Attenuation;

		TagSpotLight() : TagLight(), Eposition(0.0f, 0.0f, 0.0f), Direction(0.0f, 0.0f, -1.0f), Cutoff(30.0f), Attenuation() {

		}
		TagSpotLight(const TagSpotLight& tsl) {

			Eposition = tsl.Eposition;
			Direction = tsl.Direction;
			Cutoff = tsl.Cutoff;
			Color = tsl.Color;
			AmbientIntensity = tsl.AmbientIntensity;
			DiffuseIntensity = tsl.DiffuseIntensity;
			Attenuation = tsl.Attenuation;
		}
		TagSpotLight& operator = (const TagSpotLight& tsl)
		{
			if (&tsl == this)
			{
				return *this;
			}
			else {
				Eposition = tsl.Eposition;
				Direction = tsl.Direction;
				Cutoff = tsl.Cutoff;
				Color = tsl.Color;
				AmbientIntensity = tsl.AmbientIntensity;
				DiffuseIntensity = tsl.DiffuseIntensity;
				Attenuation = tsl.Attenuation;
				return *this;
			}
		}
	}SpotLighter;
}
