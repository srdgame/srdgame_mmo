#ifndef _CRYPTO_H_
#define _CRYPTO_H_
namespace ro
{
bool check_encrypted(const char* str1, const char* str2, const char* passwd);
bool check_password(const char* md5key, int passwd_enc, const char* passwd, const char* ref_passwd);
}
#endif
