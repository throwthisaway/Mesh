#pragma once
#include <vector>
#include <list>
#include <memory>
#include <map>
#include "VectorMath.h"
#include "IItem.h"
#include <wtypes.h>
#include "GLProgram.h"

class CVTest1
{
	static const int MAX_SPHERES = 15, RN = 3;
	static const FLOAT SPHERE_ALPHA, COLOR_BASE;
	FLOAT bbox[6], rmin, rmax, r[RN];
	typedef struct Sphere
	{
		Vector<FLOAT> v;
	//	static bool less(const FLOAT& a, const FLOAT& b) { return a < b; }
		std::multimap< FLOAT, Sphere*> neighbours;	// ordered by ascending distance
		Sphere(Vector<FLOAT>& v) : v(v) {};
		FLOAT d(const Vector<FLOAT>& s);
	}Sphere;
	std::vector<Sphere> spheres;
	std::vector<Vector<FLOAT>> sphereColors;
	Vector<FLOAT> centerColor;
	bool Contains(const Vector<FLOAT>& s);
	void InitData(void);
public:
	CVTest1();
	~CVTest1();
	std::vector<std::unique_ptr<Scene::IItem>> GenerateSpheres(Scene::ProgramMap& shaders);
};

