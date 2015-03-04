#ifdef _MSC_VER
#if _MSC_VER<1800
#include "rint.h"
#include <cmath>

double rint(double x)
{
    //middle value point test
    if (ceil(x+0.5) == floor(x+0.5))
	{
	    int a = (int)ceil(x);
	    if (a%2 == 0)
		{return ceil(x);}
	    else
		{return floor(x);}
	}

    else return floor(x+0.5);
}
#endif
#endif

