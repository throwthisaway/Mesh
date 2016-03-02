#pragma once
#include <math.h>

template<class T> class Vector
{
public:
	// TODO:: possible alignment problem on x64???
	union
	{
		T arr[4];
		#pragma pack(1)//4?
		struct
		{
			T x;
			T y;
			T z;
			union
			{
				T w;
				T r;
			};
			
		};
		#pragma pack()
	};
	Vector() {};
	Vector(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {};
	Vector(T _x, T _y, T _z) : x(_x), y(_y), z(_z), w(0) {};
	inline void operator+= (const Vector<T>& vec) { x += vec.x; y += vec.y; z += vec.z; };	
	inline void operator/= (const T r) { x /= r; y /= r; z /= r; };
	inline void operator-= (const Vector<T>& v) { x -= v.x; y -= v.y; z -= v.z; };
	inline operator const T * () const { return arr; };
	inline Vector<T> operator- (const Vector<T>& v)
	{
		return Vector<T>(x - v.x, y - v.y, z - v.z);
	}
};
template<class T> class Vector3
{
public:
	// TODO:: possible alignment problem on x64???
	union
	{
		T arr[3];
		#pragma pack(1)//4?
		struct
		{
			T x;
			T y;
			T z;
		};
		#pragma pack()
	};
	Vector3() {};
	Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {};
	inline void operator+= (const Vector<T>& vec) { x += vec.x; y += vec.y; z += vec.z; };	
	inline void operator/= (const T r) { x /= r; y /= r; z /= r; };
	inline void operator-= (const Vector<T>& v) { x -= v.x; y -= v.y; z -= v.z; };
	inline operator const T * () const { return arr; };
	inline Vector3<T> operator- (const Vector3<T>& v)
	{
		return Vector3<T>(x - v.x, y - v.y, z - v.z);
	}
	inline T Length()
	{
		return sqrt(x*x + y*y + z*z);
	}
	inline void Normalize()
	{
		T r;
		r = (T)sqrt(x*x+y*y+z*z);
		if (r>0)
			*this /= r;
	};
};

template<class T> class CMatrix
{
public:
	union
	{
		T arr[16];
	#pragma pack(1)//4?
		Vector<T> x, y, z, w;
	#pragma pack()
	};
	void Identity(void)
	{
		x.x = y.y = z.z = w.w = x.w = y.w = z.w = 1.0f;
		x.y = x.z = y.x = y.z = z.x = z.y = w.x = w.y = w.z = 0.0f;
	}
	// 0 1 2 3
	// 4 5 6 7
	// 8 9 0 1
	CMatrix<T>&& Invert(void)
	{
		CMatrix<T> m;
		m.arr[0] = arr[0]; m.arr[1] = arr[4]; m.arr[2] = arr[8]; m.arr[3] = 1.f / arr[3];
		m.arr[4] = arr[1]; m.arr[5] = arr[5]; m.arr[6] = arr[9]; m.arr[7] = 1.f / arr[7];
		m.arr[8] = arr[2]; m.arr[9] = arr[6]; m.arr[10] = arr[10]; m.arr[11] = 1.f / arr[11];
		m.arr[12] = arr[12] * arr[0] - arr[13] * arr[1] - arr[14] * arr[2];
		m.arr[13] = arr[12] * arr[4] - arr[13] * arr[5] - arr[14] * arr[6];
		m.arr[14] = arr[12] * arr[8] - arr[13] * arr[9] - arr[14] * arr[10];
		// TODO:: translation 
		return m;
	}
};

template<class T> class CVectorMath
{
public:

	static T Dot(const Vector<T>& vec1, const Vector<T>& vec2 )
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	};
	static Vector<T>& Cross(const Vector<T>& vec1, const Vector<T>& vec2 )
	{
		Vector<T> res;
		res.x = vec1.y * vec2.z - vec1.z * vec2.y;
		res.y = vec1.z * vec2.x - vec1.x * vec2.z;
		res.z = vec1.x * vec2.y - vec1.y * vec2.x;
		return res;
	};
	static void Cross(Vector<T>& res, const Vector<T>& vec1, const Vector<T>& vec2 )
	{
		res.x = vec1.y * vec2.z - vec1.z * vec2.y;
		res.y = vec1.z * vec2.x - vec1.x * vec2.z;
		res.z = vec1.x * vec2.y - vec1.y * vec2.x;
	};
	static void Normalize(Vector<T>& vec)
	{
		T r;
		r = (T) sqrt(Dot(vec, vec));
		if (r>0)
			vec /= r;			
	};
	static T Angle(const Vector<T>& vec1, const Vector<T>& vec2)
	{
		return (T)acos(Dot(vec1, vec2));
	}

};
