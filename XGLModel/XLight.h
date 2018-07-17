#pragma once
#include "XGLModel.h"
#include "xgl\Vec3f";

namespace XGLModel {
	typedef struct TagLight
	{
		TagLight() :Color(1.0f, 1.0f, 1.0f), AmbientIntensity(0.2), DiffuseIntensity(0.3)
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

	typedef struct TagDirectionLight : TagLight
	{
		TagDirectionLight() :Direction(0.0f, 0.0f, -1.0f)
		{

		}
		TagDirectionLight(const TagDirectionLight& tagLight) 
		{
			Color = tagLight.Color;
			AmbientIntensity = tagLight.AmbientIntensity;
			DiffuseIntensity = tagLight.DiffuseIntensity;
			Direction = tagLight.Direction;
		}

		TagDirectionLight& operator = (const TagDirectionLight& ta)
		{
			if (&ta == this)
			{
				return *this;
			}
			else {
				Color = ta.Color;
				AmbientIntensity = ta.AmbientIntensity;
				DiffuseIntensity = ta.DiffuseIntensity;
				Direction = ta.Direction;
				return *this;
			}
		}

		XGL::Vec3f Direction;
	}DirectionLighter;

	typedef struct TagPointLight: TagLight
	{
		XGL::Vec3f Eposition;
		TagAttenuation Attenuation;
		TagPointLight():TagLight(), Eposition(0.0f, 0.0f, 0.0f), Attenuation(){

		}
		TagPointLight(const TagPointLight& tagLight)
		{
			Color = tagLight.Color;
			AmbientIntensity = tagLight.AmbientIntensity;
			DiffuseIntensity = tagLight.DiffuseIntensity;
			Eposition = tagLight.Eposition;
			Attenuation = tagLight.Attenuation;
		}

		TagPointLight& operator = (const TagPointLight& tagLight)
		{
			if (&tagLight == this)
			{
				return *this;
			}
			else {
				Color = tagLight.Color;
				AmbientIntensity = tagLight.AmbientIntensity;
				DiffuseIntensity = tagLight.DiffuseIntensity;
				Eposition = tagLight.Eposition;
				Attenuation = tagLight.Attenuation;
				return *this;
			}
		}
	}PointLighter;

	typedef struct TagSpotLight : public TagPointLight {
		XGL::Vec3f Direction;
		float Cutoff;

		TagSpotLight() : TagPointLight(), Direction(0.0f, 0.0f, -1.0f), Cutoff(30.0f){

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
