#pragma once

#include "XGLModel.h"
#include "xgl\Vec3f"
#include "xgl\Vec2f"

namespace XGLModel {

	struct Vertex
	{
		Vertex(){}
		~Vertex(){}

		Vertex(const XGL::Vec3f& pos, const XGL::Vec2f& texcoord) {
			this->pos = pos;
			this->texcoord = texcoord;
			this->normal = XGL::Vec3f(0.0f, 0.0f, 0.0f);
		}
		static void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount)
		{
			for (unsigned int i = 0; i < IndexCount; i += 3) {
				unsigned int Index0 = pIndices[i];
				unsigned int Index1 = pIndices[i + 1];
				unsigned int Index2 = pIndices[i + 2];
				XGL::Vec3f v1 = pVertices[Index1].pos - pVertices[Index0].pos;
				XGL::Vec3f v2 = pVertices[Index2].pos - pVertices[Index0].pos;
				XGL::Vec3f Normal = v1^v2;
				Normal.normalize();

				pVertices[Index0].normal += Normal;
				pVertices[Index1].normal += Normal;
				pVertices[Index2].normal += Normal;
			}

			for (unsigned int i = 0; i < VertexCount; i++) {
				pVertices[i].normal.normalize();
			}
		}

		static void GetPtr(Vertex* pVerts, unsigned int vertC, float* attribs ) {

			for (int i = 0; i < vertC; ++i)
			{
				int j = 0;
				attribs[i * 8 + j] = pVerts[i].pos.x();

				++j;
				attribs[i * 8 + j] = pVerts[i].pos.y();

				++j;
				attribs[i * 8 + j] = pVerts[i].pos.z();

				++j;
				attribs[i * 8 + j] = pVerts[i].texcoord.x();

				++j;
				attribs[i * 8 + j] = pVerts[i].texcoord.y();

				++j;
				attribs[i * 8 + j] = pVerts[i].normal.x();

				++j;
				attribs[i * 8 + j] = pVerts[i].normal.y();

				++j;
				attribs[i * 8 + j] = pVerts[i].normal.z();
			}
		
		}
		XGL::Vec3f pos;
		XGL::Vec2f texcoord;
		XGL::Vec3f normal;
	};

}
