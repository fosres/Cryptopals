#if 0

CryptoPals recommends I operate on raw bytes. So that is what I will

do.

I will first convert hex encoding to a raw byte array.

I will next convert that to a base64 encoded string.

https://crypto.stackexchange.com/questions/30209/developing-algorithm-for-detecting-plain-text-via-frequency-analysis

#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <float.h>

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

#if 0
The below scoring algorithm works because the more frequent

a byte is in digital English plaintext the higher a frequency

it will have.

For example an alphabetic character will have a higher frequency

in digital English plaintext than a special, non-printable ASCII

character.

The scoring algorithm simply adds the occurence of each character

in a candidate string by its expected frequency in English plaintext.

The candidate string with the highest score is the correct English

plaintext.

This means a real English plaintext string is much more likely

to have a higher score than a string containing non-printable

ASCII chracters, for example.

The freqency table was generated using the program file

freq_table.c which analyzes The Complete Works of Shakespeare

from Project Gutenberg.
#endif

double score(uint8_t * raw,size_t rawlen)
{
	double freqtable[256];

	for ( size_t i = 0 ; i < 256 ; i++ )
	{
		freqtable[i] = 0.0;
	}
	
	freqtable[0] = 0.000000001;
	freqtable[1] = 0.000000001;
	freqtable[2] = 0.000000001;
	freqtable[3] = 0.000000001;
	freqtable[4] = 0.000000001;
	freqtable[5] = 0.000000001;
	freqtable[6] = 0.000000001;
	freqtable[7] = 0.000000001;
	freqtable[8] = 0.000000001;
	freqtable[9] = 0.000000709;
	freqtable[10] = 0.000000001;
//	freqtable[10] = 0.034830918; // existence of newline insignificant
	freqtable[11] = 0.000000001;
	freqtable[12] = 0.000000001;
	freqtable[13] = 0.034830918;
	freqtable[14] = 0.000000001;
	freqtable[15] = 0.000000001;
	freqtable[16] = 0.000000001;
	freqtable[17] = 0.000000001;
	freqtable[18] = 0.000000001;
	freqtable[19] = 0.000000001;
	freqtable[20] = 0.000000001;
	freqtable[21] = 0.000000001;
	freqtable[22] = 0.000000001;
	freqtable[23] = 0.000000001;
	freqtable[24] = 0.000000001;
	freqtable[25] = 0.000000001;
	freqtable[26] = 0.000000001;
	freqtable[27] = 0.000000001;
	freqtable[28] = 0.000000001;
	freqtable[29] = 0.000000001;
	freqtable[30] = 0.000000001;
	freqtable[31] = 0.000000001;
	freqtable[32] = 0.147608639;
	freqtable[33] = 0.001384405;
	freqtable[34] = 0.000000001;
	freqtable[35] = 0.000000177;
	freqtable[36] = 0.000000355;
	freqtable[37] = 0.000000177;
	freqtable[38] = 0.000008513;
	freqtable[39] = 0.000000177;
	freqtable[40] = 0.000041500;
	freqtable[41] = 0.000041500;
	freqtable[42] = 0.000003192;
	freqtable[43] = 0.000000001;
	freqtable[44] = 0.016798897;
	freqtable[45] = 0.000692912;
	freqtable[46] = 0.016207075;
	freqtable[47] = 0.000001064;
	freqtable[48] = 0.000027489;
	freqtable[49] = 0.000054802;
	freqtable[50] = 0.000032278;
	freqtable[51] = 0.000019154;
	freqtable[52] = 0.000027844;
	freqtable[53] = 0.000017558;
	freqtable[54] = 0.000026071;
	freqtable[55] = 0.000014365;
	freqtable[56] = 0.000026248;
	freqtable[57] = 0.000016316;
	freqtable[58] = 0.000607783;
	freqtable[59] = 0.002291379;
	freqtable[60] = 0.000000001;
	freqtable[61] = 0.000000001;
	freqtable[62] = 0.000000001;
	freqtable[63] = 0.002021096;
	freqtable[64] = 0.000000001;
	freqtable[65] = 0.007721700;
	freqtable[66] = 0.002339087;
	freqtable[67] = 0.003172106;
	freqtable[68] = 0.002272048;
	freqtable[69] = 0.006043602;
	freqtable[70] = 0.001923198;
	freqtable[71] = 0.001768016;
	freqtable[72] = 0.002963541;
	freqtable[73] = 0.009164808;
	freqtable[74] = 0.000347609;
	freqtable[75] = 0.000967629;
	freqtable[76] = 0.003632333;
	freqtable[77] = 0.002521936;
	freqtable[78] = 0.004170062;
	freqtable[79] = 0.004593577;
	freqtable[80] = 0.001843745;
	freqtable[81] = 0.000211225;
	freqtable[82] = 0.004001401;
	freqtable[83] = 0.005248890;
	freqtable[84] = 0.006790783;
	freqtable[85] = 0.002045038;
	freqtable[86] = 0.000556351;
	freqtable[87] = 0.002959639;
	freqtable[88] = 0.000059945;
	freqtable[89] = 0.001270013;
	freqtable[90] = 0.000097189;
	freqtable[91] = 0.000754807;
	freqtable[92] = 0.000000001;
	freqtable[93] = 0.000754453;
	freqtable[94] = 0.000000001;
	freqtable[95] = 0.001720663;
	freqtable[96] = 0.000000001;
	freqtable[97] = 0.047645262;
	freqtable[98] = 0.009074891;
	freqtable[99] = 0.013225445;
	freqtable[100] = 0.026075082;
	freqtable[101] = 0.080458276;
	freqtable[102] = 0.013410067;
	freqtable[103] = 0.011210024;
	freqtable[104] = 0.042648033;
	freqtable[105] = 0.039099942;
	freqtable[106] = 0.000544114;
	freqtable[107] = 0.005772077;
	freqtable[108] = 0.028587973;
	freqtable[109] = 0.018465645;
	freqtable[110] = 0.042375621;
	freqtable[111] = 0.054690012;
	freqtable[112] = 0.009142994;
	freqtable[113] = 0.000500131;
	freqtable[114] = 0.041014095;
	freqtable[115] = 0.042470859;
	freqtable[116] = 0.056397551;
	freqtable[117] = 0.022400539;
	freqtable[118] = 0.006634004;
	freqtable[119] = 0.014149622;
	freqtable[120] = 0.000903250;
	freqtable[121] = 0.016428055;
	freqtable[122] = 0.000221334;
	freqtable[123] = 0.000000001;
	freqtable[124] = 0.000000001;
	freqtable[125] = 0.000000001;
	freqtable[126] = 0.000000001;
	freqtable[127] = 0.000000001;
	freqtable[128] = 0.005594726;
	freqtable[129] = 0.000000001;
	freqtable[130] = 0.000000001;
	freqtable[131] = 0.000000001;
	freqtable[132] = 0.000010109;
	freqtable[133] = 0.000000001;
	freqtable[134] = 0.000001064;
	freqtable[135] = 0.000003370;
	freqtable[136] = 0.000000001;
	freqtable[137] = 0.000001596;
	freqtable[138] = 0.000000001;
	freqtable[139] = 0.000000001;
	freqtable[140] = 0.000000001;
	freqtable[141] = 0.000000001;
	freqtable[142] = 0.000000001;
	freqtable[143] = 0.000000001;
	freqtable[144] = 0.000000001;
	freqtable[145] = 0.000000001;
	freqtable[146] = 0.000000001;
	freqtable[147] = 0.000003015;
	freqtable[148] = 0.000444265;
	freqtable[149] = 0.000000001;
	freqtable[150] = 0.000000001;
	freqtable[151] = 0.000000001;
	freqtable[152] = 0.000066152;
	freqtable[153] = 0.004672144;
	freqtable[154] = 0.000000001;
	freqtable[155] = 0.000000001;
	freqtable[156] = 0.000220448;
	freqtable[157] = 0.000189766;
	freqtable[158] = 0.000000001;
	freqtable[159] = 0.000000001;
	freqtable[160] = 0.000002483;
	freqtable[161] = 0.000000001;
	freqtable[162] = 0.000011173;
	freqtable[163] = 0.000000001;
	freqtable[164] = 0.000000001;
	freqtable[165] = 0.000000001;
	freqtable[166] = 0.000012060;
	freqtable[167] = 0.000005853;
	freqtable[168] = 0.000006739;
	freqtable[169] = 0.000007803;
	freqtable[170] = 0.000001064;
	freqtable[171] = 0.000000709;
	freqtable[172] = 0.000000001;
	freqtable[173] = 0.000000001;
	freqtable[174] = 0.000000355;
	freqtable[175] = 0.000000001;
	freqtable[176] = 0.000000001;
	freqtable[177] = 0.000000001;
	freqtable[178] = 0.000000001;
	freqtable[179] = 0.000000001;
	freqtable[180] = 0.000000001;
	freqtable[181] = 0.000000001;
	freqtable[182] = 0.000000001;
	freqtable[183] = 0.000000001;
	freqtable[184] = 0.000000001;
	freqtable[185] = 0.000000001;
	freqtable[186] = 0.000000001;
	freqtable[187] = 0.000000177;
	freqtable[188] = 0.000000001;
	freqtable[189] = 0.000000001;
	freqtable[190] = 0.000000001;
	freqtable[191] = 0.000000177;
	freqtable[192] = 0.000000001;
	freqtable[193] = 0.000000001;
	freqtable[194] = 0.000000001;
	freqtable[195] = 0.000043628;
	freqtable[196] = 0.000000001;
	freqtable[197] = 0.000003015;
	freqtable[198] = 0.000000001;
	freqtable[199] = 0.000000001;
	freqtable[200] = 0.000000001;
	freqtable[201] = 0.000000001;
	freqtable[202] = 0.000000001;
	freqtable[203] = 0.000000001;
	freqtable[204] = 0.000000001;
	freqtable[205] = 0.000000001;
	freqtable[206] = 0.000000001;
	freqtable[207] = 0.000000001;
	freqtable[208] = 0.000000001;
	freqtable[209] = 0.000000001;
	freqtable[210] = 0.000000001;
	freqtable[211] = 0.000000001;
	freqtable[212] = 0.000000001;
	freqtable[213] = 0.000000001;
	freqtable[214] = 0.000000001;
	freqtable[215] = 0.000000001;
	freqtable[216] = 0.000000001;
	freqtable[217] = 0.000000001;
	freqtable[218] = 0.000000001;
	freqtable[219] = 0.000000001;
	freqtable[220] = 0.000000001;
	freqtable[221] = 0.000000001;
	freqtable[222] = 0.000000001;
	freqtable[223] = 0.000000001;
	freqtable[224] = 0.000000001;
	freqtable[225] = 0.000000001;
	freqtable[226] = 0.005604125;
	freqtable[227] = 0.000000001;
	freqtable[228] = 0.000000001;
	freqtable[229] = 0.000000001;
	freqtable[230] = 0.000000001;
	freqtable[231] = 0.000000001;
	freqtable[232] = 0.000000001;
	freqtable[233] = 0.000000001;
	freqtable[234] = 0.000000001;
	freqtable[235] = 0.000000001;
	freqtable[236] = 0.000000001;
	freqtable[237] = 0.000000001;
	freqtable[238] = 0.000000001;
	freqtable[239] = 0.000000177;
	freqtable[240] = 0.000000001;
	freqtable[241] = 0.000000001;
	freqtable[242] = 0.000000001;
	freqtable[243] = 0.000000001;
	freqtable[244] = 0.000000001;
	freqtable[245] = 0.000000001;
	freqtable[246] = 0.000000001;
	freqtable[247] = 0.000000001;
	freqtable[248] = 0.000000001;
	freqtable[249] = 0.000000001;
	freqtable[250] = 0.000000001;
	freqtable[251] = 0.000000001;
	freqtable[252] = 0.000000001;
	freqtable[253] = 0.000000001;
	freqtable[254] = 0.000000001;
	freqtable[255] = 0.000000001;

	double freqtable_document[256];

	for ( size_t i = 0 ; i < 256 ; i++ )
	{
		freqtable_document[i] = 0.00;
	}

	for ( size_t i = 0 ; i < rawlen ; i++ )
	{
		freqtable_document[raw[i]] += 1.0;
	}

	for ( size_t i = 0 ; i < 256 ; i++ )
	{
		freqtable_document[i] /= rawlen;

		printf("freqtable_doc[i]: i: %zu ; %.9lf\n",i,

		freqtable_document[i]);
	}

	double chi_squared_score = 0.0;

	double increment = 0.0 , diff = 0.0;

	for ( size_t i = 0; i < 256 ; i++ )
	{

		diff = freqtable_document[i] - freqtable[i];


		increment =		( 

						( diff * diff )					

						/

						freqtable[i]

					)

					;

		chi_squared_score += increment;
					
	}
	
	return chi_squared_score;

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

	double current_score = 0, min_score = DBL_MAX;

	memset(best_candidate,0x00,ctlen * sizeof(uint8_t));
		
	uint8_t candidate[ctlen];

	for	( 

			; 

			(iter <= 0xff) 

			; 

			iter++, key++

		)
	{
		memset(candidate,0x00,ctlen * sizeof(uint8_t));
		
		for ( size_t i = 0 ; i < ctlen ; i++ )
		{
			candidate[i] = ciphertext[i] ^ key;
		}

		current_score = score(candidate,ctlen);

		if ( current_score < min_score )
		{
			min_score = current_score;

			memset(best_candidate,0x00,ctlen * sizeof(uint8_t));

			memcpy(best_candidate,candidate,ctlen);
		}	
		
		printf("\nscore:%.9lf\ncandidate str:\n%s\n\n",

		current_score,candidate);

	}
		
	printf("\nscore:%.9lf\nbest_candidate:\n%s\n\n",min_score,best_candidate);
}


int main(int argc, unsigned char * argv[])
{
	if ( argc != 2 )
	{
		fprintf(stderr,"Error: Incorrect number of CLI args.");

		fprintf(stderr,"\nAborting.\n");

		exit(1);
	}

	size_t rawlen = 0;
	
	uint8_t * raw = NULL;
	
	size_t ptlen = 0;

	size_t i = 0;

	raw =	hex_to_raw(
				argv[1]

				,

				strnlen(argv[1],1000000)
				
				,

				&rawlen

	);
		
	printf("rawlen: %zu\n",i,rawlen);

	uint8_t candidate[rawlen];

	memset(candidate,0x00,rawlen * sizeof(uint8_t));
		
	decrypt(raw,rawlen,candidate);
	
	free(raw);

	return 0;
}
