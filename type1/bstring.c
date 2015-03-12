/*
 * A simple memset() in case your ANSI C does not provide it
 */
#if defined (_MSC_VER) &&  _MSC_VER>=1800
// This is not allowd to define intristic functions
#else

memset(void *s, int c, int length)
{  char *p = s;
  
   while (length--) *(p++) = c;
}

#endif
