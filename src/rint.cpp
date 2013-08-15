#ifdef _MSC_VER
#include "rint.h"
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

