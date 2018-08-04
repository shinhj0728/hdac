//
//  sha256
//
//  Implementation of SHA256 hash function.
//  Original author: Tom St Denis, tomstdenis@gmail.com, http://libtom.org
//  Modified by WaterJuice retaining Public Domain license.
//
//  This is free and unencumbered software released into the public domain - June 2013 waterjuice.org
//

#ifndef SHA256_H
#define SHA256_H


#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint64_t    length;
    uint32_t    state[8];
    uint32_t    curlen;
    uint8_t     buf[64];
} Sha256Context;

#define SHA256_HASH_SIZE           ( 256 / 8 )

typedef struct
{
    uint8_t      bytes [SHA256_HASH_SIZE];
} SHA256_HASH;


//
//  PUBLIC FUNCTIONS
//

//
//  Initialises a SHA256 Context. Use this to initialise/reset a context.
//
void	Sha256Initialise(Sha256Context *Context);

//
//  Adds data to the SHA256 context. This will process the data and update the internal state of the context. Keep on
//  calling this function until all the data has been added. Then call Sha256Finalise to calculate the hash.
//
void	Sha256Update(Sha256Context *Context, const void *Buffer, uint32_t BufferSize);

//
//  Performs the final calculation of the hash and returns the digest (32 byte buffer containing 256bit hash). After
//  calling this, Sha256Initialised must be used to reuse the context.
//
void	Sha256Finalise(Sha256Context *Context, SHA256_HASH *Digest);


#endif //SHA256_H

