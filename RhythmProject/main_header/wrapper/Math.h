#pragma once
#include "DxLib.h"
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <complex>
#include <type_traits>

namespace Math
{
	constexpr float Pi = 3.1415926535f;
	constexpr float TwoPi = Pi * 2.0f;
	constexpr float PiOver2 = Pi / 2.0f;
	constexpr float Infinity = std::numeric_limits<float>::infinity();
	constexpr float NegInfinity = -std::numeric_limits<float>::infinity();

	inline float ToRadians(float degrees)
	{
		return degrees * Pi / 180.0f;
	}

	inline float ToDegrees(float radians)
	{
		return radians * 180.0f / Pi;
	}

	inline bool NearZero(float val, float epsilon = 0.001f)
	{
		if (fabs(val) <= epsilon)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename T>
	constexpr T Max(const T& a, const T& b)
	{
		return (a < b ? b : a);
	}

	template<typename T>
	constexpr T Min(const T & a, const T & b)
	{
		return(a < b ? a : b);
	}

	template<typename T>
	constexpr T Clamp(const T & value, const T & lower, const T & upper)
	{
		return Min(upper, Max(lower, value));
	}

	inline float Abs(float value)
	{
		return fabs(value);
	}

	inline float Cos(float angle)
	{
		return cosf(angle);
	}

	inline float Sin(float angle)
	{
		return sinf(angle);
	}

	inline float Tan(float angle)
	{
		return tanf(angle);
	}

	inline float Acos(float value)
	{
		return acosf(value);
	}

	inline float Atan2(float y, float x)
	{
		return atan2f(y, x);
	}

	inline float Cot(float angle)
	{
		return 1.0f / Tan(angle);
	}

	template<class value>
	constexpr inline value Lerp(value a, value b, value f)
	{
		return a + f * (b - a);
	}

	inline float Sqrt(float value)
	{
		return sqrtf(value);
	}

	inline float Distance3D(VECTOR v1, VECTOR v2)
	{
		return Math::Sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
	}

	inline float Distance2D(VECTOR v1, VECTOR v2)
	{
		return Math::Sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
	}

	template<class value>
	constexpr inline value Fmod(value numer, value denom)
	{
		return fmod(numer, denom);
	}

	static VECTOR UnitX = VGet(1.0f, 0.0f, 0.0f);
	static VECTOR UnitY = VGet(0.0f, 1.0f, 0.0f);
	static VECTOR UnitZ = VGet(0.0f, 0.0f, 1.0f);
	static VECTOR NegUnitX = VGet(-1.0f, 0.0f, 0.0f);
	static VECTOR NegUnitY = VGet(0.0f, -1.0f, 0.0f);
	static VECTOR NegUnitZ = VGet(0.0f, 0.0f, -1.0f);

	//translate axis
	template<class vec>
	constexpr vec VectorTransAxis(vec trans) { return VGet(trans.x, trans.z, trans.y); }
}

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
		, w(1.0f)
	{
		*this = Quaternion::Identity;
	}

	//set directrly quaternion
	explicit Quaternion(float inX, float inY, float inZ, float inW)
		: x(inX)
		, y(inY)
		, z(inZ)
		, w(inW)
	{
		Set(inX, inY, inZ, inW);
	}

	//set quaternion from axis and angle
	//axis->normalized
	//angle->radians
	explicit Quaternion(const VECTOR& axis, float angle)
	{
		float scalar = Math::Sin(angle / 2.0f);
		x = axis.x * scalar;
		y = axis.y * scalar;
		z = axis.z * scalar;
		w = Math::Cos(angle / 2.0f);
	}

	//Set Directly
	void Set(float inX, float inY, float inZ, float inW)
	{
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	void Conjugate()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}

	float LengthSq() const
	{
		return(x * x + y * y + z * z + w * w);
	}

	float Length() const
	{
		return Math::Sqrt(LengthSq());
	}

	void Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	static Quaternion Normalize(const Quaternion & q)
	{
		Quaternion retVal = q;
		retVal.Normalize();
		return retVal;
	}

	//Linear interpolation
	static Quaternion Lerp(const Quaternion & a, const Quaternion & b, float f)
	{
		Quaternion retVal;
		retVal.x = Math::Lerp(a.x, b.x, f);
		retVal.y = Math::Lerp(a.y, b.y, f);
		retVal.z = Math::Lerp(a.z, b.z, f);
		retVal.w = Math::Lerp(a.w, b.w, f);
		retVal.Normalize();
		return retVal;
	}

	static float Dot(const Quaternion & a, const Quaternion & b)
	{
		return a.x* b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	//Spherical Linear Interpolation
	static Quaternion Slerp(const Quaternion & a, const Quaternion & b, float f)
	{
		float rawCosm = Quaternion::Dot(a, b);

		float cosom = -rawCosm;
		if (rawCosm >= 0.0f)
		{
			cosom = rawCosm;
		}

		float scale0, scale1;
		if (cosom < 0.9999f)
		{
			const float omega = Math::Acos(cosom);
			const float invSin = 1.f / Math::Sin(omega);
			scale0 = Math::Sin((1.f - f) * omega) * invSin;
			scale1 = Math::Sin(f * omega) * invSin;
		}
		else
		{
			//if the quaternions are collinear, use linear intepolation
			scale0 = 1.0f - f;
			scale1 = f;
		}

		if (rawCosm < 0.0f)
		{
			scale1 = -scale1;
		}

		Quaternion retVal;
		retVal.x = scale0 * a.x + scale1 * b.x;
		retVal.y = scale0 * a.y + scale1 * b.y;
		retVal.z = scale0 * a.z + scale1 * b.z;
		retVal.w = scale0 * a.w + scale1 * b.w;
		retVal.Normalize();
		return retVal;
	}

	//concatenate
	//Rotate by q FOLLOWED by p
	static Quaternion Concatenate(const Quaternion & q, const Quaternion & p)
	{
		Quaternion retVal;

		//vector component is:
		// ps * qv + qs * pv + pv x qv
		VECTOR qv = VGet(q.x, q.y, q.z);
		VECTOR pv = VGet(p.x, p.y, p.z);
		VECTOR newVec = VAdd(VScale(qv, p.w), VAdd(VScale(pv, q.w), VCross(pv, qv)));
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;

		//scalar component is:
		//ps * qs - pv . qv
		retVal.w = p.w * q.w - VDot(pv, qv);
		return retVal;
	}

	static const Quaternion Identity;

};

namespace Math::QuaternionLibrary
{
	VECTOR Transform(const VECTOR& v, const Quaternion& q);
	MATRIX CreateFromQuaternion(const Quaternion& q);
	bool CreateFromMatrix(MATRIX q, Quaternion& rot);
}

namespace Math::MineCraft
{
	//ex. size -> chank size -> 16, scale -> model scale -> 100

	//とある数値からscale x sizeの長さで区切った点の最大値
	template<class T>
	constexpr T MultipleRangeInt(T num, T size, T scale)
	{
		T ans = 0;
		while (std::abs(num) < size * scale * ans)
		{
			ans++;
		}
		return num > 0 ? ans * size * scale : (-ans + 1) * scale * size;
	}
}

namespace Math::NoiseLibrary
{
	//seed値を設定してそれからハッシュ値を生成して返す
	//返される型はstd::array<int,256>
	template<class value>
	constexpr std::array<value, 512> GetHash(value seed)
	{
		SRand(seed); //乱数生成、乱数はseedが同じなら全く同じ値となる
		std::array<value, 256> permutation;
		for (int i = 0; i < permutation.size(); i++)
		{
			permutation[i] = GetRand(255); //0~255の値を格納
		}

		//この順列からhashを生成
		std::array<value, 512> hash;
		for (int i = 0; i < hash.size(); i++)
		{
			hash[i] = permutation[i % 256];
		}

		//hashを返す
		return hash;
	}

	//勾配間を緩やかに移行する
	template<class value>
	constexpr inline value fade(value t)
	{
		return t * t* t* (t * (t * 6 - 15) + 10);
	}

	template<class value>
	constexpr value inc(value num, value repeat)
	{
		num++;
		if (repeat > 0) { num %= repeat; }
		return num;
	}

	//----------   3D perlin   ----------
	//勾配ベクトルと8つの位置ベクトルのドット積
	template<class value>
	constexpr inline value grad3D(int hash, value x, value y, value z)
	{
		switch (hash & 0xF)
		{
		case 0x0: return  x + y;
		case 0x1: return -x + y;
		case 0x2: return  x - y;
		case 0x3: return -x - y;
		case 0x4: return  x + z;
		case 0x5: return -x + z;
		case 0x6: return  x - z;
		case 0x7: return -x - z;
		case 0x8: return  y + z;
		case 0x9: return -y + z;
		case 0xA: return  y - z;
		case 0xB: return -y - z;
		case 0xC: return  y + x;
		case 0xD: return -y + z;
		case 0xE: return  y - x;
		case 0xF: return -y - z;
		default: return 0;
		}
	}

	template<class value>
	constexpr value perlin3D(value x, value y, value z, int repeat, std::array<int, 512> p)
	{
		if (repeat > 0) {
			x = Math::Fmod<float>(x, repeat);
			y = Math::Fmod<float>(y, repeat);
			z = Math::Fmod<float>(z, repeat);
		}

		int xi = static_cast<int>(x) & 255;
		int yi = static_cast<int>(y) & 255;
		int zi = static_cast<int>(z) & 255;
		value xf = x - static_cast<int>(x);
		value yf = y - static_cast<int>(y);
		value zf = z - static_cast<int>(z);
		value u = Math::NoiseLibrary::fade<value>(xf);
		value v = Math::NoiseLibrary::fade<value>(yf);
		value w = Math::NoiseLibrary::fade<value>(zf);

		int aaa, aba, aab, abb, baa, bba, bab, bbb;
		aaa = p[p[p[xi] + yi] + zi];
		aba = p[p[p[xi] + Math::NoiseLibrary::inc<int>(yi, repeat)] + zi];
		aab = p[p[p[xi] + yi] + Math::NoiseLibrary::inc<int>(zi, repeat)];
		abb = p[p[p[xi] + Math::NoiseLibrary::inc<int>(yi, repeat)] + Math::NoiseLibrary::inc<int>(zi, repeat)];
		baa = p[p[p[Math::NoiseLibrary::inc<int>(xi, repeat)] + yi] + zi];
		bba = p[p[p[Math::NoiseLibrary::inc<int>(xi, repeat)] + Math::NoiseLibrary::inc<int>(yi, repeat)] + zi];
		bab = p[p[p[Math::NoiseLibrary::inc<int>(xi, repeat)] + yi] + Math::NoiseLibrary::inc<int>(zi, repeat)];
		bbb = p[p[p[Math::NoiseLibrary::inc<int>(xi, repeat)] + Math::NoiseLibrary::inc<int>(yi, repeat)] + Math::NoiseLibrary::inc<int>(zi, repeat)];

		value x1, x2, y1, y2;
		x1 = Math::Lerp<value>(Math::NoiseLibrary::grad3D<value>(aaa, xf, yf, zf), Math::NoiseLibrary::grad3D<value>(baa, xf - 1, yf, zf), u);
		x2 = Math::Lerp<value>(Math::NoiseLibrary::grad3D<value>(aba, xf, yf - 1, zf), Math::NoiseLibrary::grad3D<value>(bba, xf - 1, yf - 1, zf), u);
		y1 = Math::Lerp<value>(x1, x2, v);
		x1 = Math::Lerp<value>(Math::NoiseLibrary::grad3D<value>(aab, xf, yf, zf - 1), Math::NoiseLibrary::grad3D<value>(bab, xf - 1, yf, zf - 1), u);
		x2 = Math::Lerp<value>(Math::NoiseLibrary::grad3D<value>(abb, xf, yf - 1, zf - 1), Math::NoiseLibrary::grad3D<value>(bbb, xf - 1, yf - 1, zf - 1), u);
		y2 = Math::Lerp<value>(x1, x2, v);

		return (Math::Lerp<value>(y1, y2, w) + 1) / 2;
	}

	//octave:繰り返し回数,persistence:octaveの影響力
	template<class value>
	constexpr value OctavePerlin3D(value x, value y, value z, int octaves, value persistence,int repeat,std::array<int,512> p)
	{
		value total = 0;
		value frequency = 1;
		value amplitude = 1;
		value maxValue = 0;
		for (int i = 0; i < octaves; i++)
		{
			total += Math::NoiseLibrary::perlin3D<float>(x * frequency, y * frequency, z * frequency, repeat, p) * amplitude;
			maxValue += amplitude;
			
			amplitude *= persistence; //persistence^i
			frequency *= 2; //2^i
		}
		return total / maxValue;
	}
	//-----------------------------------
}

namespace Math::SignalLibrary
{
	using comp = std::complex<double>;

	template<class Container>
	auto DFS(const Container& fourier)
		->decltype(auto)
	{
		const int N = std::size(fourier); // size
		const Container::value_type i(0, 1);
		int t, x;
		Container F(N); //DFS calculate answer
		for (t = 0; t < N; t++)
		{
			F[t] = 0;
			for (x = 0; x < N; x++)
			{
				F[t] += fourier[x] * std::exp(-i * static_cast<typename Container::value_type::value_type>(2.0f * Math::Pi * t * x / N));
			}
		}

		return F;
	}
	
	template<class Container>
	auto InverseDFS(const Container& fourier)
	{
		const int N = std::size(fourier);
		const Container::value_type i(0, 1);
		int t, x;
		Container f(N);

		for (x = 0; x < N; ++x)
		{
			f[x] = 0;
			for (t = 0; t < N; ++t)
			{
				f[x] += fourier[t] * std::exp(i * static_cast<typename Container::value_type::value_type>(2.0f * Math::Pi * t * x / N));
			}
			f[x] /= N;
		}
		return f;
	}
}