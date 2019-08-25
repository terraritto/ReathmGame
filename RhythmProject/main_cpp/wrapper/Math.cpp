#include "../../main_header/wrapper/Math.h"

const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

namespace Math::QuaternionLibrary {
	VECTOR Transform(const VECTOR& v, const Quaternion& q)
	{
		VECTOR qv = VGet(q.x, q.y, q.z);
		VECTOR retVal = v;
		retVal = VAdd(retVal, VScale(VCross(qv, VAdd(VCross(qv, v), VScale(v, q.w))), 2.0f));
		return retVal;
	}

	MATRIX CreateFromQuaternion(const Quaternion& q)
	{
		MATRIX mat;
		

		mat.m[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
		mat.m[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
		mat.m[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
		mat.m[0][3] = 0.0f;

		mat.m[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
		mat.m[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
		mat.m[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
		mat.m[1][3] = 0.0f;

		mat.m[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
		mat.m[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
		mat.m[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
		mat.m[2][3] = 0.0f;

		mat.m[3][0] = 0.0f;
		mat.m[3][1] = 0.0f;
		mat.m[3][2] = 0.0f;
		mat.m[3][3] = 1.0f;

		return mat;
	}

	bool CreateFromMatrix(const MATRIX q, Quaternion& rot)
	{
		//ç≈ëÂê¨ï™Çåüçı
		float elem[4];// 0:x,1:y,2:z,3:w
		elem[0] = q.m[0][0] - q.m[1][1] - q.m[2][2] + 1.0f;
		elem[1] = -q.m[0][0] + q.m[1][1] - q.m[2][2] + 1.0f;
		elem[2] = -q.m[0][0] - q.m[1][1] + q.m[2][2] + 1.0f;
		elem[3] = q.m[0][0] + q.m[1][1] + q.m[2][2] + 1.0f;

		unsigned biggestIndex = 0;
		for (int i = 1; i < 4; i++)
		{
			if (elem[i] > elem[biggestIndex])
			{
				biggestIndex = i;
			}
		}

		//à¯êîÇÃçsóÒÇ…ä‘à·Ç¢Ç™Ç†ÇÈèÍçáÇÕî≤ÇØÇÈ
		if (elem[biggestIndex] < 0.0f)
		{
			return false;
		}
		float q_ans[4] = { rot.x,rot.y,rot.z,rot.w };
		float v = Math::Sqrt(elem[biggestIndex]) * 0.5f;
		q_ans[biggestIndex] = v;
		float mult = 0.25f / v;
		switch (biggestIndex)
		{
		case 0: //x
			q_ans[1] = (q.m[0][1] + q.m[1][0]) * mult;
			q_ans[2] = (q.m[2][0] + q.m[0][2]) * mult;
			q_ans[3] = (q.m[1][2] - q.m[2][1]) * mult;
			break;
		case 1:
			q_ans[0] = (q.m[0][1] + q.m[1][0]) * mult;
			q_ans[2] = (q.m[1][2] + q.m[2][1]) * mult;
			q_ans[3] = (q.m[2][0] - q.m[0][2]) * mult;
			break;
		case 2:
			q_ans[0] = (q.m[2][0] + q.m[0][2]) * mult;
			q_ans[1] = (q.m[1][2] + q.m[2][1]) * mult;
			q_ans[3] = (q.m[0][1] - q.m[1][0]) * mult;
			break;
		case 3:
			q_ans[0] = (q.m[1][2] - q.m[2][1]) * mult;
			q_ans[1] = (q.m[2][0] - q.m[0][2]) * mult;
			q_ans[2] = (q.m[0][1] - q.m[1][0]) * mult;
			break;
		}
		rot.Set(q_ans[0], q_ans[1], q_ans[2], q_ans[3]);

		return true;
	}
}