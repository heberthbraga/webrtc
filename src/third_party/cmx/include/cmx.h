/*
cmx.h - CMX ANSI C header - Cache Mapping Acceleration library:
Author: Luis Paris
Copyright (c) Centri Technology, Inc.
Copyright (c) The University of Mississippi.
*/

#ifndef INCLUDE_CMX_H
#define INCLUDE_CMX_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//
//flush values

#define CMX_NO_FLUSH        0   //unsupported (not needed)
#define CMX_PARTIAL_FLUSH   1   //unsupported (not needed)
#define CMX_SYNC_FLUSH      2
#define CMX_FULL_FLUSH      3
#define CMX_FINISH          4

//
//return values

#define CMX_NEED_DICT       2
#define CMX_STREAM_END      1
#define CMX_OK              0
#define CMX_ERRNO         (-1)
#define CMX_STREAM_ERROR  (-2)
#define CMX_DATA_ERROR    (-3)
#define CMX_MEM_ERROR     (-4)
#define CMX_BUF_ERROR     (-5)
#define CMX_VERSION_ERROR (-6)
#define CMX_ARGS_ERROR    (-7)
#define CMX_CIPHER_ERROR  (-8)
#define CMX_CACHE_ERROR   (-9)
#define CMX_SEED_ERROR   (-10)

//
//rollback values

#define CMX_LAST_FLUSH      0
#define CMX_LAST_CALL       1   //unsupported (not needed)

//
//cmx metric constants

#define CMX_K      13  //default K value
#define CMX_LINES   8  //default Lines value (power of 2)

//
//utility macros

#define CMX_CACHE_BYTES( k, lines)  ((1 << ((k) ? (k) : CMX_K)) * ((lines) ? (lines) : CMX_LINES))
#define CMX_OLEN_BYTES( ilen) ((ilen) + (ilen) / 8 + 128)

//
//cmx_data struc.

typedef struct {
    unsigned k;  //k param in cmx <n,k,lines,h> metrics (n=8, h=3 are constants)
    unsigned lines;  //lines param in cmx <n,k,lines,h> metrics
    unsigned h;  //h param in cmx <n,k,lines,h> metrics (currently ignored, h=3 constant)
    unsigned cachelen;  //external cache length in bytes; use CMX_CACHE_BYTES macro if needed
    void *cache;  //ptr to external cache alloc./managed by caller; pass NULL for internal alloc.
    int seedlen;  //seed length in bytes; or seed index within seed array if NULL seed; if 0, no seed
    int ivlen;  //length of initialization vector in bytes; if ivlen <= 0, no cipher
    const void *seed;  //ptr to seed; if NULL, seedlen is seed index
    const void *iv;  //ptr to initialization vector; if iv=NULL or iv={0..(ivlen bytes)..0}, no cipher
    void *(*cmxalloc)( void *opaque, int items, int size);  //external alloc func.
    void (*cmxfree)( void *opaque, void *mem);  //external free func.
    void *opaque;  //opaque ptr (useful to pass app. specific data)
    uint64_t total_in;  //total #bytes read so far from ibuf
    uint64_t total_out;  //total #bytes written so far to obuf
    void *state;  //internal codec state, handled internally (read only)
    //todo: add cipher related fields below
} cmx_data;

//
//cmx memory API

int  //CMX_OK if successful; *ilen: #bytes processed from ibuf; *olen: #bytes written to obuf; cmx->{seed,seedlen} = 0
cmx_encode(  //encodes (compresses/decrypts) source stream
    const void *ibuf,  //ibuf: ptr to input buffer containing source stream to be processed
    int *ilen,         //*ilen: #bytes to process from ibuf
    void *obuf,        //obuf: ptr to output buffer where encoded stream will be written to
    int *olen,         //*olen: max #bytes to write to obuf (to avoid overflow)
    cmx_data *cmx,     //cmx->{k,lines,... etc.} pass codec params (see cmx_data struct. for fields usage)
    int flush);        //see flush values

int  //CMX_OK if successful
cmx_encodeInit( cmx_data *cmx);  //(optional) cmx_encode() performs initialization on first call

int  //CMX_OK if successful
cmx_encodeEnd( cmx_data *cmx);  //(optional) cmx_decode() performs cleanup automatically if cmx_decodeInit() was invoked
                                //explicitly, and if and when cmx_encode(..., flush = CMX_FINISH) returns CMX_STREAM_END.

int  //CMX_OK if successful; *ilen: #bytes processed from ibuf; *olen: #bytes written to obuf; cmx->{seed,seedlen} = 0
cmx_decode(  //decodes (compresses/decrypts) cmx stream
    const void *ibuf,  //ibuf: ptr to input buffer containing cmx stream to be processed
    int *ilen,         //*ilen: #bytes to process from ibuf
    void *obuf,        //obuf: ptr to output buffer where decoded stream will be written to
    int *olen,         //*olen: max #bytes to write to obuf (to avoid overflow)
    cmx_data *cmx);    //cmx->{k,lines,... etc.} pass codec params (see cmx_data struct. for fields usage)

int  //CMX_OK if successful
cmx_decodeInit( cmx_data *cmx);  //(optional) cmx_decode() performs initialization on first call

int  //CMX_OK if successful
cmx_decodeEnd( cmx_data *cmx);  //(optional) cmx_decode() performs cleanup automatically if cmx_decodeInit() was invoked
                                //explicitly, and if and when cmx_encode(..., flush = CMX_FINISH) returns CMX_STREAM_END.

//
//cipher key storage API

int  //CMX_OK if successful, CMX_CIPHER_ERROR if cipher not initialized or not enabled
cmx_saveKey( cmx_data *cmx);  //save current value of internal key within cipher state (stored internally)

int  //CMX_OK if successful, CMX_CIPHER_ERROR if cipher not initialized or not enabled
cmx_loadKey( cmx_data *cmx);  //load previously saved value of internal key within cipher state

//
//state rollback API

int  //CMX_OK if successful
cmx_rollback(  //rolls cache state back to last flush point or encode call
    int last_event,    //CMX_LAST_FLUSH or CMX_LAST_CALL
    cmx_data *cmx);    //cmx->cache

#ifdef __cplusplus
}
#endif

#endif  /* _CMX_H */
