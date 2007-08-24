/******

GameSpy / GameMaster Utility Code
  
Copyright 1998 Critical Mass Communications, LLC.

Suite E-204
2900 Bristol Street
Costa Mesa, CA 92626
(714)549-7689
Fax(714)549-0757


******/

#define GAMESPY_VALIDATE_SIZE 6

/**********
gs_encode: This function converts a binary buffer to printable text.
It uses the base64 algorithm, which expands 3 byte pairs to 4 bytes.

IN
--
ins: a pointer to the buffer of data you want to encode
size: size of the buffer

OUT
---  
result: pointer to buffer to store result. Size should be (4 * size) / 3 + 1
	result will be null terminated.
**********/
void gs_encode(unsigned char  *ins, int size, unsigned char  *result);

/**********
gs_decode: This function decodes a base64 buffer into a binary stream.
4 byte pairs are compressed to 3 bytes.
  
IN
--
ins: a pointer to the buffer of data you want to decode

OUT
---  
result: pointer to buffer to store result. Size should be (3 * size) / 4 
	result will NOT be null terminated unless you encoded a null character.
**********/
void gs_decode(unsigned char  *ins, unsigned char  *result);

/**********
gs_encrypt: This functions encypts a buffer (in place) with a given
key. The key is assumed to be a null terminated string.
NOTE: Encypting the buffer a second time with the same key will
decrypt it.


IN
--
buffer_ptr: buffer to be encrypted
buffer_len: size of the buffer
key: null terminated string containing the key to encode with.

OUT
---  
buffer_ptr: buffer, encrypted in place
**********/
void gs_encrypt(unsigned char *buffer_ptr,int buffer_len, unsigned char *key);
