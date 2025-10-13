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
#include <stdbool.h>
#include <ctype.h>

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

			size_t * rawlen

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
		fprintf(stderr,"Error: hexstr does not encode");

		fprintf(stderr," for a full byte array since");
		
		fprintf(stderr," the number of hex digits\n"); 
		
		fprintf(stderr," %zu is not a multiple of 2.\n",hexstrlen);
		
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

		size_t rawlen_size =	( hexstrlen / 2 );


		;

		uint8_t * raw_byte_arr = (uint8_t *)calloc(
								
								rawlen_size + 1,

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

	*rawlen = rawlen_size;

	return raw_byte_arr;

}

bool is_str_print(const unsigned char * str, size_t len)
{
	if ( str[len] != 0x00 )
	{
		fprintf(stderr,"Error: string is not null-terminated");

		fprintf(stderr," at str[len].\nAborting.\n");

		exit(1);
	}

	for (size_t i = 0 ; i < len ; i++ )
	{
		if ( isprint(str[i]) == false )
		{
			return false;
		}	
	} 		

	return true;
}

size_t score(uint8_t * raw,size_t rawlen)
{
	size_t scoretable[256];

	memset(scoretable,0x00,256*sizeof(size_t));

	scoretable['a'] = 817;

	scoretable['b'] = 150;

	scoretable['c'] = 278;

	scoretable['d'] = 425;

	scoretable['e'] = 1270;

	scoretable['f'] = 223;

	scoretable['g'] = 202;

	scoretable['h'] = 609;

	scoretable['i'] = 697;

	scoretable['j'] = 15;

	scoretable['k'] = 77;

	scoretable['l'] = 403;

	scoretable['m'] = 241;

	scoretable['n'] = 675;

	scoretable['o'] = 751;

	scoretable['p'] = 193;

	scoretable['q'] = 10;

	scoretable['r'] = 599;

	scoretable['s'] = 633;

	scoretable['t'] = 906;

	scoretable['u'] = 276;

	scoretable['v'] = 98;

	scoretable['w'] = 236;

	scoretable['x'] = 15;

	scoretable['y'] = 197;

	scoretable['z'] = 7;

	size_t score = 0;

	for ( size_t i = 0 ; i < rawlen ; i++ )
	{
		score += scoretable[tolower(raw[i])];	
	}	

	return score;

}


#if 0
There is a single byte key.

There are only 256 possible bytes.

And one of them *has* to be the key.

The decrypt() will brute-force guess the key by cecking each one of

the possible one-byte keys against the ciphertext. The first byte

decrypted that is not printable results in the rejection of that

key.

Technically it is possible for there to be a key that does not

decrypt into legible English plaintext--just printable ASCII

characters.

So the below function prints all printable ASCII printable strings

derived. The correct key will yield a legible plaintext.
#endif

void decrypt(
			const uint8_t * ciphertext

			,

			const size_t ctlen

			,

			uint8_t * best_candidate

		)
{
	size_t iter = 0, i = 0;

	unsigned char key = 0;

	size_t current_score = 0, max_score = 0;

	memset(best_candidate,0x00,ctlen * sizeof(uint8_t));

	for	( 

			; 

			(iter <= 0xff) 

			; 

			iter++, key++

		)
	{


		uint8_t candidate[ctlen];

		memset(candidate,0x00,ctlen * sizeof(uint8_t));
		
		for ( size_t i = 0 ; i < ctlen ; i++ )
		{
			candidate[i] = ciphertext[i] ^ key;
		}

		current_score = score(candidate,ctlen);

		if ( current_score > max_score )
		{
			max_score = current_score;

			memset(best_candidate,0x00,ctlen * sizeof(uint8_t));

			memcpy(best_candidate,candidate,ctlen);
		}	

	}
}


int main(int argc, unsigned char * argv[])
{
	if ( argc != 2 )
	{
		fprintf(stderr,"Error: Incorrect number of CLI args.");

		fprintf(stderr,"\nAborting.\n");

		exit(1);
	}

	FILE * file = NULL;

	FILE * file_cp = NULL;

	unsigned char buf[61];

	size_t scores_ptarr[327];

	memset(scores_ptarr,0x00,327 * sizeof(size_t));

	uint8_t ptarr[327][61];

	memset(ptarr,0x00,327 * 61 * sizeof(uint8_t));

	memset(buf,0x00,61*sizeof(unsigned char));

	if	( 
			( file = fopen(argv[1],"r") ) == NULL 
		)
	{
		fprintf(stderr,"Error: Failed to open file.\n");

		exit(1);			
	}
	
	if	( 
			( file_cp = fopen("/tmp/copy.txt","w+") ) == NULL 
		)
	{
		fprintf(stderr,"Error: Failed to open file.\n");

		exit(1);			
	}

	size_t rawlen = 0;
	
	uint8_t * raw = NULL;
	
	size_t ptlen = 0;

	size_t i = 0;

	size_t buflen = 0;

	while ( fgets(buf,61,file) != NULL )
	{
		buflen = strnlen(buf,61);
		
		printf("buflen: %zu\n\nbuf[buflen-1]: %.2x\n",buflen,buf[buflen-1]);

		if ( buf[0] == 0x0a )
		{
			memset(buf,0x00,61*sizeof(unsigned char));

			continue;
		}

		if ( buf[buflen-1] == 0x0a )
		{
			buf[buflen-1] = 0x00;

			buflen--;
		}
		
		printf("buf[buflen-1]: %.2x\n",buf[buflen-1]);
		
		printf("\nbuf: %s\n\n",buf);

		raw =	hex_to_raw(
					buf

					,

					buflen
					
					,

					&rawlen

		);
			
		printf("Text Line Number: %zu\trawlen: %zu\n",i,rawlen);
		
		printf("\nbuf: %s\n\n",buf);

		fprintf(file_cp,"%s\n",buf);
			
		decrypt(raw,rawlen,ptarr[i]);

		memset(raw,0x00,rawlen * sizeof(uint8_t));
		
		free(raw);

		memset(buf,0x00,61*sizeof(unsigned char));

		i++;
	}

	for ( size_t i = 0 ; i < 327 ; i++ )
	{
		printf("%s\n\n",ptarr[i]);
	}

	uint8_t dptarr[327][61];

	memset(dptarr,0x00,327 * 61 * sizeof(uint8_t));

	for ( size_t i = 0 ; i < 327 ; i++ )
	{
		decrypt(ptarr[i],strnlen(ptarr[i],61),dptarr[i]);
	}
	
	printf("dptarr output:\n\n");
	
	for ( size_t i = 0 ; i < 327 ; i++ )
	{
		printf("%s\n\n",dptarr[i]);
	}
	
	fclose(file);
	
	fclose(file_cp);

	return 0;
}
