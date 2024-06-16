#pragma once
namespace TYPE{
template <class T> class Range
{
	T Min;
	T Max;
	T interval;
public:
	Range(T mi, T ma)
	{
		Min = min(mi,ma);
		Max = max(mi,ma);
	}
	
	Range(T mi, T ma, T in)
	{
		Min = min(mi,ma);
		Max = max(mi,ma);
		interval = in;
	}

	Range(T m):Min(m), Max(m)
	{
	}

	Range()
	{
	}

	T MAX(){ return Max;}

	T MIN(){ return Min; }

	T INTERVAL(){ return interval; }
};
}