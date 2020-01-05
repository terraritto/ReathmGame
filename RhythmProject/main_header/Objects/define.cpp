#include "define.h"

ETiming DecisionTiming(LONGLONG time)
{
	if (-JUDGE_TIME * 2 <= time && time <= JUDGE_TIME * 2)
	{
		return ETiming::ECritical;
	}

	if (-JUDGE_TIME * 4 <= time && time <= JUDGE_TIME * 4)
	{
		return ETiming::EBlust;
	}

	if (-JUDGE_TIME * 6 <= time && time <= JUDGE_TIME * 6)
	{
		return ETiming::EHit;
	}

	if (JUDGE_TIME * 8 <= time)
	{
		return ETiming::EMiss;
	}

	return ETiming::ENone;

}
