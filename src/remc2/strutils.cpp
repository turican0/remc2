#include "strutils.h"

int strleni(int i, int base)
{
	int len = 0;
	if(i < 0)
	{
		len++;
		i = -i;
	}

	len += base == 16 ? 2 : base == 8 ? 1 : 0;
	for(len; i > 0; i/=base, len++);

	return len;
}

std::string itoa(int i, int base)
{
	int len = strleni(i, base);

	/* allocate memory of for dst string */
	char buf[len+1];
	char* p = buf;

	/* zero terminate */
	p[len] = '\0';

	/* sign case */
	if(i < 0)
	{
		*p++ = '-';
		i = -i;
	}

	switch(base)
	{
		case 16:
			snprintf(p, len + 1, "0x%X\n", i);
			break;
		case 8:
			snprintf(p, len + 1, "O%o\n", i);
			break;
		default:
			snprintf(p, len + 1, "%d\n", i);
			break;
	}

	return std::string(buf);
}

