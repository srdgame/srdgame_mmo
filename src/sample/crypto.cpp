#include "crypto.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "md5/md5class.h"
namespace ro
{
bool check_encrypted(const char* str1, const char* str2, const char* passwd)
{
	char temp[64 + 1];
	memset(temp, 0, 64 + 1);
	char md5str[32 + 1];
	memset(temp, 0, 32 + 1);

	sprintf(temp, "%s%s", str1, str2);
	CMD5 md5(temp);
	memcpy(md5str, md5.getMD5Digest(), 32 + 1);

	return (0 == strcmp(passwd, md5str));
}

bool check_password(const char* md5key, int passwd_enc, const char* passwd, const char* ref_passwd)
{
	if (passwd_enc == 0)
	{
		return (0 == strcmp(passwd, ref_passwd));
	}
	else
	{
		return ((passwd_enc & 0x01) && check_encrypted(md5key, ref_passwd, passwd)) ||
				((passwd_enc & 0x02) && check_encrypted(ref_passwd, md5key, passwd));
	}
}
}
