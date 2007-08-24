/******

GameSpy / GameMaster Utility Code
  
Copyright 1998 Critical Mass Communications, LLC.

Suite E-204
2900 Bristol Street
Costa Mesa, CA 92626
(714)549-7689
Fax(714)549-0757

see gsutils.h for documentation

******/

#include "stdlib.h"

typedef unsigned char uchar;
typedef uchar trip_t[3];
typedef uchar kwart_t[4];

typedef struct rc4_key
{      
	uchar state[256];       
	uchar x;        
	uchar y;
} rc4_key;

static void prepare_key(uchar *key_data_ptr,int key_data_len, rc4_key *key);
static void rc4(uchar *buffer_ptr,int buffer_len,rc4_key *key);
static void swap_byte(uchar *a, uchar *b);
static void trip2kwart(trip_t trip, kwart_t kwart);
static uchar encode_ct(uchar c);
static void kwart2trip(kwart_t kwart, trip_t trip);
static uchar decode_ct(uchar c);

void gs_encrypt(uchar *buffer_ptr,int buffer_len, uchar *key)
{
	int keylen=0;
	char *hold=key;
	rc4_key akey;

	while (*hold++) keylen++;

	prepare_key(key,keylen,&akey);
	rc4(buffer_ptr, buffer_len, &akey);
}

void gs_encode(uchar *ins, int size, uchar *result)
{
	
	int i,pos;
	trip_t trip;
	kwart_t kwart;
	
	i=0;
	while (i < size)
	{
		for (pos=0 ; pos <= 2 ; pos++, i++)
			if (i < size)
				trip[pos] = *ins++;
			else
				trip[pos] = '\0';
		
		trip2kwart(trip,kwart);
		for (pos=0 ; pos <= 3 ; pos++)
			*result++ = encode_ct(kwart[pos]);
	}
	*result='\0';
}

void gs_decode(uchar *ins, uchar *result)
{
	int pos;
	trip_t trip;
	kwart_t kwart;
	
	while (*ins)
	{
		for (pos = 0; pos <= 3; pos++)
			kwart[pos] = decode_ct(*ins++);

			kwart2trip(kwart,trip);
			for (pos = 0 ; pos <= 2; pos++)
				*result++ = trip[pos];
	}
}

static void prepare_key(uchar *key_data_ptr, int key_data_len,rc4_key *key)
{
	uchar index1;
	uchar index2;
	unsigned char* state;
	short counter;     
	

	state = &key->state[0];         
	for(counter = 0; counter < 256; counter++)              
        state[counter] = (unsigned char)counter;               
	key->x = 0;     
	key->y = 0;     
	index1 = 0;     
	index2 = 0;             
	for(counter = 0; counter < 256; counter++)      
	{               
		index2 = (key_data_ptr[index1] + state[counter] +
			index2) % 256;                
		swap_byte(&state[counter], &state[index2]);            
		
		index1 = (index1 + 1) % key_data_len;  
	}       
}


void rc4(uchar *buffer_ptr, int buffer_len, rc4_key *key)
{ 
	uchar x;
	uchar y;
	unsigned char* state;
	uchar xorIndex;
	short counter;              
	
	x = key->x;     
	y = key->y;     
	
	state = &key->state[0];         
	for(counter = 0; counter < buffer_len; counter ++)      
	{               
//note: this single line has changed from the initial code
		x = (x + buffer_ptr[counter] + 1) % 256;

		y = (state[x] + y) % 256;               
		swap_byte(&state[x], &state[y]);                        
		
		xorIndex = (state[x] + state[y]) % 256;                 
		
		buffer_ptr[counter] ^= state[xorIndex];         
	}               
	key->x = x;     
	key->y = y;
}

static void swap_byte(uchar *a, uchar *b)
{
	uchar swapByte; 
	
	swapByte = *a; 
	*a = *b;      
	*b = swapByte;
}

static void trip2kwart(trip_t trip, kwart_t kwart)
{
	kwart[0] = (trip[0]) >> 2;
	kwart[1] = (((trip[0]) & 3) << 4) + ((trip[1]) >> 4);
	kwart[2] = (((trip[1]) & 15) << 2) + (((trip[2]) >> 192) >> 6);
	kwart[3] = (trip[2]) & 63;
}


static uchar encode_ct(uchar c)
{
	
	switch (c)
	{
	case 0 :return  'A' ;case 1 :return  'B' ;case 2 :return  'C' ;case 3 :return  'D' ;
	case 4 :return  'E' ;case 5 :return  'F' ;case 6 :return  'G' ;case 7 :return  'H' ;
	case 8 :return  'I' ;case 9 :return  'J' ;case 10 :return  'K' ;case 11 :return  'L' ;
	case 12 :return  'M' ;case 13 :return  'N' ;case 14 :return  'O' ;case 15 :return  'P' ;
	case 16 :return  'Q' ;case 17 :return  'R' ;case 18 :return  'S' ;case 19 :return  'T' ;
	case 20 :return  'U' ;case 21 :return  'V' ;case 22 :return  'W' ;case 23 :return  'X' ;
	case 24 :return  'Y' ;case 25 :return  'Z' ;case 26 :return  'a' ;case 27 :return  'b' ;
	case 28 :return  'c' ;case 29 :return  'd' ;case 30 :return  'e' ;case 31 :return  'f' ;
	case 32 :return  'g' ;case 33 :return  'h' ;case 34 :return  'i' ;case 35 :return  'j' ;
	case 36 :return  'k' ;case 37 :return  'l' ;case 38 :return  'm' ;case 39 :return  'n' ;
	case 40 :return  'o' ;case 41 :return  'p' ;case 42 :return  'q' ;case 43 :return  'r' ;
	case 44 :return  's' ;case 45 :return  't' ;case 46 :return  'u' ;case 47 :return  'v' ;
	case 48 :return  'w' ;case 49 :return  'x' ;case 50 :return  'y' ;case 51 :return  'z' ;
	case 52 :return  '0' ;case 53 :return  '1' ;case 54 :return  '2' ;case 55 :return  '3' ;
	case 56 :return  '4' ;case 57 :return  '5' ;case 58 :return  '6' ;case 59 :return  '7' ;
	case 60 :return  '8' ;case 61 :return  '9' ;case 62 :return  '+' ;case 63 :return  '/' ;
	default: return '0';
	}
}

static void kwart2trip(kwart_t kwart, trip_t trip)
{
	trip[0] = ((kwart[0] << 2)+(kwart[1] >> 4));
	trip[1] = ((kwart[1] << 4)+(kwart[2] >> 2));
	trip[2] = ((kwart[2] << 6)+kwart[3]);
}


static uchar decode_ct(uchar c)
{
	switch (c)
	{
	case 'A' : return 0 ;  case 'B' : return 1 ;  case 'C' : return 2 ;  case 'D' : return 3 ;
	case 'E' : return 4 ;  case 'F' : return 5 ;  case 'G' : return 6 ;  case 'H' : return 7 ;
	case 'I' : return 8 ;  case 'J' : return 9 ;  case 'K' : return 10 ;  case 'L' : return 11 ;
	case 'M' : return 12 ;  case 'N' : return 13 ;  case 'O' : return 14 ;  case 'P' : return 15 ;
	case 'Q' : return 16 ;  case 'R' : return 17 ;  case 'S' : return 18 ;  case 'T' : return 19 ;
	case 'U' : return 20 ;  case 'V' : return 21 ;  case 'W' : return 22 ;  case 'X' : return 23 ;
	case 'Y' : return 24 ;  case 'Z' : return 25 ;  case 'a' : return 26 ;  case 'b' : return 27 ;
	case 'c' : return 28 ;  case 'd' : return 29 ;  case 'e' : return 30 ;  case 'f' : return 31 ;
	case 'g' : return 32 ;  case 'h' : return 33 ;  case 'i' : return 34 ;  case 'j' : return 35 ;
	case 'k' : return 36 ;  case 'l' : return 37 ;  case 'm' : return 38 ;  case 'n' : return 39 ;
	case 'o' : return 40 ;  case 'p' : return 41 ;  case 'q' : return 42 ;  case 'r' : return 43 ;
	case 's' : return 44 ;  case 't' : return 45 ;  case 'u' : return 46 ;  case 'v' : return 47 ;
	case 'w' : return 48 ;  case 'x' : return 49 ;  case 'y' : return 50 ;  case 'z' : return 51 ;
	case '0' : return 52 ;  case '1' : return 53 ;  case '2' : return 54 ;  case '3' : return 55 ;
	case '4' : return 56 ;  case '5' : return 57 ;  case '6' : return 58 ;  case '7' : return 59 ;
	case '8' : return 60 ;  case '9' : return 61 ;  case '+' : return 62 ;  case '/' : return 63 ;
	default: return '0';
	}
	
}


