#pragma once
#include "mesh.h"
namespace Scene
{
	class CTorus :
		public CMesh
	{
		void Setup(unsigned int vbovertices) {};
        void CreateVertex(int i, int u, int v, float r1, float r2, float ds1, float ds2);
	public:
		void Setup(float r1/*r: tube radius*/, float r2/*R: torus radius*/, int s1 /*s: tube circle slice no.*/, int s2/*S: tube slice no.*/);
		virtual void Draw(void);
		CTorus(ProgramMap& shaders);
		~CTorus(void);
	};
}
