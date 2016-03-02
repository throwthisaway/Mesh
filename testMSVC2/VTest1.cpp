#include "VTest1.h"
#include "Sphere.h"
#include "Center.h"
#include "VectorMath.h"
#include "Log.h"
#include "Lines.h"
double tomb[][4]{{10.0, 5.0, 2.0, 9.0 },
{ 20.0, 12.0, 7.0, 22.0 },
{ 30.0, 19.0, 17.0, 45.0 }};
int gauss(double a[][4], int n)
{
	int i, j, k;
	double t, eps;
	eps = 0.000001;

	for (k = 0; k < n; k++)
	{
		t = a[k][k];

		if (fabs(t) > eps)
		{
			//printf("\nOszt:  a[%d,%d] = %6.2lf\n", k, k, t);
			t = 1.0 / t;
			for (i = k; i < n + 1; i++)
				a[k][i] = t*a[k][i];

			if (k != n)
			{
				//printf("\n%d.  iteráció\n", k);
				for (i = k + 1; i < n; i++)
				{
					t = a[i][k];
					for (j = k; j < n + 1; j++)
						a[i][j] = a[i][j] - a[k][j] * t;
				}
				//kiir(a, n);
			}
		}
		else return 1;
	}

	for (i = n-1; i>=0; i--)
	{
		t = a[i][n];
		for (k = i + 1; k < n; k++)
			t = t - a[i][k] * a[k][n];
		a[i][n] = t / a[i][i];
	}
	return 0;
}
const FLOAT CVTest1::SPHERE_ALPHA = 0.5f, CVTest1::COLOR_BASE = 0.5f;
FLOAT CVTest1::Sphere::d(const Vector<FLOAT>& v)
{
	FLOAT dx = this->v.x - v.x, dy = this->v.y - v.y, dz = this->v.y - v.y;
	return sqrt(dx * dx + dy * dy + dz * dz);
}
bool CVTest1::Contains(const Vector<FLOAT>& v)
{
	Log::CLog::Write(">>>\n");
	for (auto& it : spheres)
	{
		FLOAT d = it.d(v);
		Log::CLog::Write("%g %g\n", d, fabs(v.r - it.v.r));
		if (d <= fabs(v.r - it.v.r))
			return true;
	}
	return false;
}
CVTest1::CVTest1() :
	rmin(5.0f),
	rmax(10.0f),
	centerColor(1.0f, 0.0f, 1.0f, 1.0f)
{
	gauss(tomb, 3);
	bbox[0] = -10.0f;
	bbox[1] = -10.0f;
	bbox[2] = -10.0f;
	bbox[3] = 10.0f;
	bbox[4] = 10.0f;
	bbox[5] = 10.0f;

	sphereColors.reserve(6);
	sphereColors.push_back(Vector<FLOAT>(COLOR_BASE, 0.0f, 0.0f, SPHERE_ALPHA));
	sphereColors.push_back(Vector<FLOAT>(0.0f, COLOR_BASE, 0.0f, SPHERE_ALPHA));
	sphereColors.push_back(Vector<FLOAT>(0.0f, 0.0f, COLOR_BASE, SPHERE_ALPHA));
	sphereColors.push_back(Vector<FLOAT>(COLOR_BASE, COLOR_BASE, 0.0f, SPHERE_ALPHA));
	sphereColors.push_back(Vector<FLOAT>(COLOR_BASE, 0.0f, COLOR_BASE, SPHERE_ALPHA));
	sphereColors.push_back(Vector<FLOAT>(0.0f, COLOR_BASE, COLOR_BASE, SPHERE_ALPHA));

	for (int i = 0; i < RN; i++)
	{
		r[i] = (FLOAT)::rand() / (RAND_MAX + 1) * (rmax - rmin) + rmin; 
	}
}


CVTest1::~CVTest1()
{

}
void CVTest1::InitData(void)
{
	spheres.erase(spheres.cbegin(), spheres.cend());
	spheres.reserve(MAX_SPHERES);
	for (int i = 0; i < MAX_SPHERES;)
	{
		FLOAT x = (FLOAT)::rand() / (RAND_MAX + 1) * (bbox[3] - bbox[0]) + bbox[0],
			y = (FLOAT)::rand() / (RAND_MAX + 1) * (bbox[4] - bbox[1]) + bbox[1],
			z = (FLOAT)::rand() / (RAND_MAX + 1) * (bbox[5] - bbox[2]) + bbox[2],
			r = this->r[::rand() % RN];
		Vector<FLOAT> s(x, y, z, r);
		if (Contains(s))
			continue;
		i++;
		spheres.push_back(s);
	}

	// find closest neighbours...
	for (auto& s : spheres)
	{
		FLOAT mind = 0.0f;
		for (auto& s2 : spheres)
		{
			if (&s == &s2)
				continue;
			FLOAT d = s.d(s2.v) - s.v.r - s2.v.r;
			s.neighbours.insert({ d, &s2 });
		}
	}
}
std::vector<std::unique_ptr<Scene::IItem>> CVTest1::GenerateSpheres(Scene::ProgramMap& shaders)
{
	InitData();
	std::vector<std::unique_ptr<Scene::IItem>> items;
	items.reserve(MAX_SPHERES * 2);
	std::vector<FLOAT> vertices, colors;
	for (std::vector<Sphere>::iterator it = spheres.begin(); it != spheres.end(); it++)
	{
		Vector<FLOAT>& s = (*it).v;
		int cIdx = (int)(floor((s.r - rmin) / (rmax - rmin) * sphereColors.size()));
		Scene::CSphere* sphere = new Scene::CSphere(shaders, Vector3<FLOAT>(s.x, s.y, s.z), Vector3<FLOAT>(0.0f, 0.0f, 0.0f), Vector3<FLOAT>(s.w, s.w, s.w), sphereColors[cIdx]);
		Scene::CCenter* center = new Scene::CCenter(shaders, Vector3<FLOAT>(s.x, s.y, s.z), Vector3<FLOAT>(0.0f, 0.0f, 0.0f), Vector3<FLOAT>(0.5f, 0.5f, 0.5f), centerColor);
		items.push_back(std::unique_ptr<Scene::IItem>(sphere));
		items.push_back(std::unique_ptr<Scene::IItem>(center));
		vertices.push_back(s.x); vertices.push_back(s.y); vertices.push_back(s.z); colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	}
	items.push_back(std::unique_ptr<Scene::IItem>(new Scene::CLines(shaders, vertices, colors)));
	return items;
}
static void Calc(Vector<DOUBLE>& s1, Vector<DOUBLE>& s2, Vector<DOUBLE>& s3)
{
	Vector<DOUBLE> s;
	s.r = sqrt(s.x*s.x + s.y + s.y + s.z*s.z);
	DOUBLE rq = s.r * s.r - 2 * s.r * s1.r + s1.r * s1.r,
		rqinp = s.x*s.x + s.y * s.y + s.z*s.z - 2 * sqrt(s.x*s.x + s.y * s.y + s.z*s.z)  * s1.r + s1.r * s1.r;
}
