#include <iostream>
#include "log.h"
#include "color.h"

bool out_notempty = true;

void init(FILE* out, bool crfile)
{
	if(!out) out_notempty = false; 
	else out_notempty = true;
	if(crfile) out = fopen("log.txt", "a+"); // default filename is log.txt, change if necessary
	else return;
}

void loginf(FILE* out, const char* text)
{
	if(!out_notempty) return;
	fprintf(out, "%s[INFO]%s: %s\n",BLU, CRESET, text);
}

void logwarn(FILE* out, const char* text)
{
	if(!out_notempty) return;
	fprintf(out, "%s[WARN]%s: %s\n",YEL, CRESET, text);
}

void logerr(FILE* out, const char* text)
{
	if(!out_notempty) return;
	fprintf(out, "%s[ERROR]%s: %s\n",RED, CRESET, text);
}
