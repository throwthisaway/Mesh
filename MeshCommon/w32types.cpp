char * strlwr(char * s)
{
	char * p = s;
	if (!p)
		return 0;
	while (*p)
	{
		if ((*p<='Z') && (*p>='A'))
			*p += 0x20;
		p++;		
	}
	return s;
}