#if 0

CryptoPals recommends I operate on raw bytes. So that is what I will

do.

I will first convert hex encoding to a raw byte array.

I will next convert that to a base64 encoded string.

#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#if 0

Below function converts hex encoded string to a raw byte array.

Remember that two hex digits is a byte. Whereas a single hex

digit is 4 bits of data.

This fact matters because when we convert a hex-encoded string

we will be converting the hex-encoded string to a raw byte array.

Remember it takes *two* hex digits to fill up a byte of raw data.

When we hit the end of the hex-encoded string we may run into one

of two situations:

1. We are left with one hex digit that needs to be translated to

a raw byte array.

2. We have finished appending the last two hex digits into the

raw byte array.

The above two situations are possible when we convert two hex digits

into raw bytes at a time.

In other words: n hex digits % (modulus) 2 == 0 or 1

If we end up with a remaining *single* hex digit at the end of

the hex-encoded string we will have to translate that into a byte.

Since a single hex digit encodes for 4 binary digits these 4 binary

digits must be the least remaining binary digits in the last byte.

In other words the first 4 binary digits of this last byte will be

0000. The last 4 binary digits will be the binary equivalent of the

last hex digit.

So the form of the last byte will be: 0000xxxx

In hex_to_raw() the hexstr[] can be as large as 1 Megabyte in size

which is hereby defined as 1,000,000 bytes in size.

There is a reason why I am setting a hard limit on how long hexstr[]

can be. If a string is too long the computer can waste excessive

time scanning the string and then converting. There are even

cyberattacks that can take place if the computer takes too long to

scan strings: The Long String Password Attack.

https://marty-k.medium.com/denial-of-service-creating-mayhem-was-never-so-easy-3406a9c518d5

#endif

uint8_t * hex_to_raw	(
			
			const unsigned char hexstr[],

			size_t hexstrlen,

			size_t * arg1len

)
{

#if 0
Below is a conditional statement that tests if the number of

hex digits in the hex string is a multiple of 2.

This is a must for the hexstring to encode a complete byte array.

CryptoPals insists we operate on raw bytes.

And Base64 encoding converts raw bytes to base64 characters.
#endif
	if ( hexstrlen % 2 != 0 )
	{
		fprintf(stderr,"Error: hexstr \"%s\" does not encode"

		,

		hexstr

		);

		fprintf(stderr," for a full byte array since");
		
		fprintf(stderr," the number of hex digits\n"); 
		
		fprintf(stderr,"is not a multiple of 2.\n");
		
		fprintf(stderr,"Aborting.\n");

		exit(1);
	}

#if 0
It is possible to convert a single null byte to a Base64-Encoded

String.

Based on RFC 4648 (https://www.rfc-editor.org/rfc/rfc4648#section-4)

when there is a single raw byte remaining (even a null byte) Base64

will append two Base64 characters and two padding ('=') characters.

So a null byte is: AA==

#endif
	if ( hexstrlen == 0 )
	{
		fprintf(stderr,"Error: hexstr is a null string.");

		fprintf(stderr,"Aborting\n");

		exit(1);
		
	}

	if ( hexstrlen > 1000000 )
	{
		fprintf(stderr,"Error: hexstr too long. Aborting\n");

		exit(1);
	}

	if (
		( hexstr[hexstrlen] != 0x00 )
	)
	{
		fprintf(stderr,

		"Error: Null byte not set at end of hexstr string!\n");

		exit(1);
	}
	
	// raw byte array should be of length:
	
	// ceiling(hexstrlen / 2)

	// https://en.wikipedia.org/wiki/Floor_and_ceiling_functions

	size_t arg1len_size =	( hexstrlen / 2 );


	;

	uint8_t * raw_byte_arr = (uint8_t *)calloc(
							
							arg1len_size + 1,

							sizeof(uint8_t)
						)

	;

	unsigned char hex_list[256];

	memset(hex_list,0xff,256 * sizeof(unsigned char));

	hex_list['0'] = 0;

	hex_list['1'] = 1;
	
	hex_list['2'] = 2;
	
	hex_list['3'] = 3;
	
	hex_list['4'] = 4;
	
	hex_list['5'] = 5;
	
	hex_list['6'] = 6;
	
	hex_list['7'] = 7;
	
	hex_list['8'] = 8;
	
	hex_list['9'] = 9;
	
	hex_list['a'] = 10;
	
	hex_list['b'] = 11;
	
	hex_list['c'] = 12;
	
	hex_list['d'] = 13;
	
	hex_list['e'] = 14;
	
	hex_list['f'] = 15;

	for ( size_t i = 0 ; i < hexstrlen ; i++ )
	{
		if ( hex_list[hexstr[i]] == 0xff )
		{
		
			fprintf(stderr,

				"Error: Invalid hex character \'%c\'.\n",

				hexstr[i]

			);
			
			fprintf(stderr,

				"Aborting.\n"
			);

			exit(1);

					
		}

		// Even numbered hex characters are the 4 most

		// significant bits in a byte

		// Odd numbered hex characters are the 4 least

		// significant bits in a byte

		raw_byte_arr[i / 2]	|=	

					( i % 2 == 0 )

					? 
					
					hex_list[hexstr[i]] << 4		
					
					:

					hex_list[hexstr[i]] 

					;	 
	}

	*arg1len = arg1len_size;

	return raw_byte_arr;

}

unsigned char * raw_to_hex(
				uint8_t * raw

				,

				const size_t rawlen

				,

				size_t * hexlen
			
				)
{

	(*hexlen) = rawlen * 2;

	unsigned char * hexarr = (unsigned char*)calloc(

						(*hexlen) + 1	

						,

						sizeof(unsigned char)

						);


	if ( hexarr == NULL )
	{
		fprintf(stderr,"Error: hexarr in raw_to_hex() failed");

		fprintf(stderr,"\nto initialize. Aborting.\n");

		exit(1);
	}

	unsigned char hextable[16];

	memset(hextable,0x00,16 * sizeof(unsigned char));

	hextable[0] = '0';
	
	hextable[1] = '1';
	
	hextable[2] = '2';
	
	hextable[3] = '3';
	
	hextable[4] = '4';
	
	hextable[5] = '5';
	
	hextable[6] = '6';
	
	hextable[7] = '7';
	
	hextable[8] = '8';
	
	hextable[9] = '9';
	
	hextable[0xa] = 'a';
	
	hextable[0xb] = 'b';
	
	hextable[0xc] = 'c';
	
	hextable[0xd] = 'd';
	
	hextable[0xe] = 'e';
	
	hextable[0xf] = 'f';

	for ( size_t rawi = 0, hexi = 0 ; rawi < rawlen ; rawi++, hexi += 2 )
	{
		hexarr[hexi] = hextable[raw[rawi] >> 4];	

		hexarr[hexi + 1] = hextable[raw[rawi] & 0b1111];
	}

	return hexarr;
}

int main(int argc, unsigned char * argv[])
{
	if ( argc != 3 )
	{
		fprintf(stderr,"Error: Invalid number of arguments.\n");

		fprintf(stderr,"Aborting.\n");

		exit(1);	
	}
	
	size_t arg1len = 0;

	uint8_t * raw_arr = hex_to_raw(
	
					argv[1],

					strnlen(argv[1],1000000),

					&arg1len

					);		
	printf("arg[1]: ");

	for ( size_t i = 0; i < arg1len ; i++ )
	{
		printf("%.2x",raw_arr[i]);
	}

	printf("\n");

	size_t arg2len = 0;

	uint8_t * raw_arr2 = hex_to_raw(
	
					argv[2],

					strnlen(argv[2],1000000),

					&arg2len

					);		
	printf("arg[2]: ");

	for ( size_t i = 0; i < arg2len ; i++ )
	{
		printf("%.2x",raw_arr2[i]);
	}

	printf("\n");

	if ( arg1len != arg2len )
	{
		fprintf(stderr,"Error: arg1len != arg2len.");

		fprintf(stderr,"\nAborting.\n");

		exit(1);
	}

	uint8_t xor_arr[arg1len];

	memset(xor_arr,0x00,arg1len * sizeof(unsigned char));	

	for ( size_t i = 0; i < arg1len ; i++ )
	{
		xor_arr[i] = raw_arr[i] ^ raw_arr2[i];
	}
	
	size_t hexlen = 0;

	unsigned char * hexarr = raw_to_hex(xor_arr,arg1len,&hexlen);

	printf("xorarr: %s\n",hexarr);

	free(raw_arr);

	free(raw_arr2);

	free(hexarr);
	
	return 0;
}
