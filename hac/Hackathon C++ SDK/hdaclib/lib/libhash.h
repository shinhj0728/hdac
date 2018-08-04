//
// Copyright (c) 2018 Hdac Technology AG
// Hdac code distributed under the GPLv3 license
//============================================================================================
// libhash.h
//	Hash functions
//
// History
//	2018/01/10	Initial code
//============================================================================================
//

#ifndef __LIBHASH_H__
#define __LIBHASH_H__


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


#include "base58.h"
#include "md5.h"
#include "rc4.h"
#include "sha1.h"
#include "sha256.h"
#include "sha512.h"


#define TOHEXDIGIT(v)	((v) < 10 ? (v)+'0' : (v)+('a'-10))


// hash functions
char	*sha1(char *outhash, const char *str, const size_t len);
char	*sha256(char *outhash, const char *str, const size_t len);
char	*sha512(char *outhash, const char *str, const size_t len);
char	*md5(char *outhash, const char *str, const size_t len);

char	*bin2hex(char *hexbuf, const void *binbuf, const size_t binbuflen);
void	*hex2bin(void *binbuf, const char *hexbuf, const size_t hexbuflen);


#endif	// __LIBHASH_H__
