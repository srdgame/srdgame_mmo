#ifndef STR_LIB_H_
#define STR_LIB_H_

bool bin2hex(char* out, unsigned char* in, size_t size);

#define SEX_FEMALE 0
#define sex_num2str(num) ( (num ==  SEX_FEMALE  ) ? 'F' : (num ==  SEX_MALE  ) ? 'M' : 'S' )
#define sex_str2num(str) ( (str == 'F' ) ?  SEX_FEMALE  : (str == 'M' ) ?  SEX_MALE  :  SEX_SERVER  )

#endif
