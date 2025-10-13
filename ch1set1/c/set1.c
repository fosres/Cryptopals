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

void base64_en_24_bits	(
				
				uint8_t * raw,

				size_t rawi,
				
				unsigned char base64_table[],

				unsigned char base64_en_raw[],

				size_t * base64i

				);

void base64_en_last_8_bits	(
				
				uint8_t * raw,

				size_t li,
				
				unsigned char base64_table[],

				unsigned char base64_en_raw[],

				size_t * base64i

				);

void base64_en_last_16_bits	(
				
				uint8_t * raw,

				size_t li,

				unsigned char base64_table[],

				unsigned char base64_en_raw[],

				size_t * base64i

				);

unsigned char * raw_to_base64	(

				uint8_t * raw,

				size_t rawlen,

				size_t * base64_len

				)
{

// below base64_table maps binary values to Base64 characters

	unsigned char base64_table[64];

	size_t tablei = 0;

	unsigned char base64char = 0;

	memset(base64_table,0x00,64 * sizeof(unsigned char));

	for	(
			base64char = 'A';

			base64char <= 'Z' ;

			base64char++, tablei++

		)

	{
		base64_table[tablei] = base64char;	
	}

	for	(
			base64char = 'a';

			base64char <= 'z' ;

			base64char++, tablei++

		)

	{
		base64_table[tablei] = base64char;	
	}
			
	for	(
			base64char = '0';

			base64char <= '9' ;

			base64char++, tablei++

		)

	{
		base64_table[tablei] = base64char;	
	}

	base64_table[tablei++] = '+';
	
	base64_table[tablei++] = '/';

#if 0

From RFC 4648 Section 4:

Base64 encoding considers 24 bits (3 bytes of data) at a time.

But what if the last group of bits is not 3 bytes but less?

There are 3 possibilities since n bytes % 3 bytes == 0, 1, or 2

bytes of data.

1. If 0 bytes of data left we are done with Base64 encoding

2. If 1 byte of data is left there will be 4 binary bits appended.

This results in 2 base64 characters and 2 padding "=" characters.

3. If 2 bytes of data are left there will be 2 binary bits

appended.

This results in 3 base64 characters and 1 padding "=" characters. 
#endif

#if 0
Length of base64 array is 4 * ceil(n / 3)

Reference: https://stackoverflow.com/questions/13378815/base64-length-calculation
#endif
	double rawlen_div_by_3 = ceil(rawlen / 3.0);

	*base64_len = 4 * (size_t)rawlen_div_by_3;
	
	unsigned char * base64_en_raw = (unsigned char *)
	
					calloc(
						(*base64_len) + 1,

						sizeof(unsigned char)

					);

	;

	// unsigned char base64_en_raw[base64_len + 1];

	size_t base64i = 0;

	size_t li = 0 , ri = 3;
	
	for	( 
			;  

			( li < rawlen ) && ( ri <= rawlen )

			; 

			li += 3 , ri += 3

		)

	{
		base64_en_24_bits(
					raw,li,base64_table,

					base64_en_raw,&base64i

					);

		;	
	}

	switch ( rawlen % 3 )
	{
		case 1:
		{	

			base64_en_last_8_bits(	
						raw,li,base64_table,

						base64_en_raw,&base64i

						);
			;
	
			break;
		}
		
		case 2:
		{
			base64_en_last_16_bits(
	
						raw,li,base64_table,

						base64_en_raw,&base64i

						);

			break;
		}

		default:
		{
			break;
		}
	}

	return base64_en_raw;
}


void base64_en_24_bits	(
				
				uint8_t * raw,

				size_t rawi,
				
				unsigned char base64_table[],

				unsigned char base64_en_raw[],

				size_t * base64i

				)
{

#if 0
Take a look at the Base64 diagram for "Man" on Wikipedia as

you read the below code:

"Encoding of the source string ⟨Man⟩ in Base64" Section in the article

https://en.wikipedia.org/wiki/Base64
#endif

	unsigned char base64_char = 0;

	base64_en_raw[*base64i] = base64_table[raw[rawi] >> 2];

	base64_char = 0;

	base64_char = ( ( raw[rawi] & 0b11 ) << 4 );

	rawi++;

	(*base64i)++;

	base64_char |= ( raw[rawi] >> 4 );
	
	base64_en_raw[*base64i] = base64_table[base64_char];
	
	(*base64i)++;
	
	base64_char = 0;

	base64_char |= ( ( raw[rawi] & 0b1111 ) << 2 );
	
	rawi++;
	
	base64_char |= ( raw[rawi] >> 6 );
	
	base64_en_raw[*base64i] = base64_table[base64_char];
	
	(*base64i)++;
	
	base64_en_raw[*base64i] = base64_table[raw[rawi] & 0b00111111];

	(*base64i)++;
}

void base64_en_last_8_bits	(
				
				uint8_t * raw,

				size_t rawi,
				
				unsigned char base64_table[],

				unsigned char base64_en_raw[],

				size_t * base64i

				)
{
#if 0
See the example for Base64 encoding "M" on Wikipedia:

https://en.wikipedia.org/wiki/Base64

#endif	

	unsigned char base64_char = 0;

	base64_en_raw[*base64i] = base64_table[raw[rawi] >> 2];

	(*base64i)++;

	base64_char = ( ( raw[rawi] & 0b11 ) << 4 );

	base64_en_raw[*base64i] = base64_table[base64_char];
	
	(*base64i)++;

	base64_en_raw[*base64i] = '=';
	
	(*base64i)++;

	base64_en_raw[*base64i] = '=';
	
	(*base64i)++;

}

void base64_en_last_16_bits	(
				
				uint8_t * raw,

				size_t rawi,

				unsigned char base64_table[],

				unsigned char base64_en_raw[],

				size_t * base64i

				)
{
	unsigned char base64_char = 0;

	base64_en_raw[*base64i] = base64_table[raw[rawi] >> 2];

	base64_char = 0;

	base64_char = ( ( raw[rawi] & 0b11 ) << 4 );

	rawi++;

	(*base64i)++;

	base64_char |= ( raw[rawi] >> 4 );
	
	base64_en_raw[*base64i] = base64_table[base64_char];
	
	(*base64i)++;

	base64_char = 0;

	base64_char = ( ( raw[rawi] & 0b1111 ) << 2 );

	base64_en_raw[*base64i] = base64_table[base64_char];

	(*base64i)++;

	base64_en_raw[*base64i] = '=';

	(*base64i)++;
	
}



int main(int argc, unsigned char * argv[])
{
	if ( argc != 2 )
	{
		fprintf(stderr,"Error: Invalid number of arguments.\n");

		fprintf(stderr,"Aborting.\n");

		exit(1);	
	}
	
	size_t rawlen = 0;

	uint8_t * raw_arr = hex_to_raw(
	
					argv[1],

					strnlen(argv[1],1000000),

					&rawlen

					);		
	printf("raw_arr: ");

	for ( size_t i = 0; i < rawlen ; i++ )
	{
		printf("%.2x",raw_arr[i]);
	}

	printf("\n");
	
	size_t base64_len = 0;

	unsigned char * base64_en_raw = raw_to_base64(
							raw_arr,

							rawlen,

							&base64_len

							);	

	;

	printf("rawlen: %zu\nbase64_len: %zu\nbase64_en_raw: %s\n",

	rawlen,base64_len,base64_en_raw);

	printf("\n\n");
	
	free(raw_arr);	

	free(base64_en_raw);

	return 0;
}
