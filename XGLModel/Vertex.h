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
			this->m_pos = pos;
			this->m_tex = texcoord;
			this->m_normal = XGL::Vec3f(0.0f, 0.0f, 0.0f);
		}

		Vertex(const XGL::Vec3f& pos, const XGL::Vec2f& texcoord,const XGL::Vec3f& normal) {
			this->m_pos = pos;
			this->m_tex = texcoord;
			this->m_normal = normal;
		}
		static void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount)
		{
			for (unsigned int i = 0; i < IndexCount; i += 3) {
				unsigned int Index0 = pIndices[i];
				unsigned int Index1 = pIndices[i + 1];
				unsigned int Index2 = pIndices[i + 2];
				XGL::Vec3f v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
				XGL::Vec3f v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
				XGL::Vec3f Normal = v1^v2;
				Normal.normalize();

				pVertices[Index0].m_normal += Normal;
				pVertices[Index1].m_normal += Normal;
				pVertices[Index2].m_normal += Normal;
			}

			for (unsigned int i = 0; i < VertexCount; i++) {
				pVertices[i].m_normal.normalize();
			}
		}

		static void GetPtr(Vertex* pVerts, unsigned int vertC, float* attribs ) {

			for (int i = 0; i < vertC; ++i)
			{
				int j = 0;
				attribs[i * 8 + j] = pVerts[i].m_pos.x();

				++j;
				attribs[i * 8 + j] = pVerts[i].m_pos.y();

				++j;
				attribs[i * 8 + j] = pVerts[i].m_pos.z();

				++j;
				attribs[i * 8 + j] = pVerts[i].m_tex.x();

				++j;
				attribs[i * 8 + j] = pVerts[i].m_tex.y();

				++j;
				attribs[i * 8 + j] = pVerts[i].m_normal.x();

				++j;
				attribs[i * 8 + j] = pVerts[i].m_normal.y();

				++j;
				attribs[i * 8 + j] = pVerts[i].m_normal.z();
			}
		
		}
		XGL::Vec3f m_pos;
		XGL::Vec2f m_tex;
		XGL::Vec3f m_normal;
	};

}
