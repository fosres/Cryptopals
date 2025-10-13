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

	freqtable[0] = 0.0000000;
	freqtable[1] = 0.0000000;
	freqtable[2] = 0.0000000;
	freqtable[3] = 0.0000000;
	freqtable[4] = 0.0000000;
	freqtable[5] = 0.0000000;
	freqtable[6] = 0.0000000;
	freqtable[7] = 0.0000000;
	freqtable[8] = 0.0000000;
	freqtable[9] = 0.0000007;
	freqtable[10] = 0.0348309;
	freqtable[11] = 0.0000000;
	freqtable[12] = 0.0000000;
	freqtable[13] = 0.0348309;
	freqtable[14] = 0.0000000;
	freqtable[15] = 0.0000000;
	freqtable[16] = 0.0000000;
	freqtable[17] = 0.0000000;
	freqtable[18] = 0.0000000;
	freqtable[19] = 0.0000000;
	freqtable[20] = 0.0000000;
	freqtable[21] = 0.0000000;
	freqtable[22] = 0.0000000;
	freqtable[23] = 0.0000000;
	freqtable[24] = 0.0000000;
	freqtable[25] = 0.0000000;
	freqtable[26] = 0.0000000;
	freqtable[27] = 0.0000000;
	freqtable[28] = 0.0000000;
	freqtable[29] = 0.0000000;
	freqtable[30] = 0.0000000;
	freqtable[31] = 0.0000000;
	freqtable[32] = 0.1476086;
	freqtable[33] = 0.0013844;
	freqtable[34] = 0.0000000;
	freqtable[35] = 0.0000002;
	freqtable[36] = 0.0000004;
	freqtable[37] = 0.0000002;
	freqtable[38] = 0.0000085;
	freqtable[39] = 0.0000002;
	freqtable[40] = 0.0000415;
	freqtable[41] = 0.0000415;
	freqtable[42] = 0.0000032;
	freqtable[43] = 0.0000000;
	freqtable[44] = 0.0167989;
	freqtable[45] = 0.0006929;
	freqtable[46] = 0.0162071;
	freqtable[47] = 0.0000011;
	freqtable[48] = 0.0000275;
	freqtable[49] = 0.0000548;
	freqtable[50] = 0.0000323;
	freqtable[51] = 0.0000192;
	freqtable[52] = 0.0000278;
	freqtable[53] = 0.0000176;
	freqtable[54] = 0.0000261;
	freqtable[55] = 0.0000144;
	freqtable[56] = 0.0000262;
	freqtable[57] = 0.0000163;
	freqtable[58] = 0.0006078;
	freqtable[59] = 0.0022914;
	freqtable[60] = 0.0000000;
	freqtable[61] = 0.0000000;
	freqtable[62] = 0.0000000;
	freqtable[63] = 0.0020211;
	freqtable[64] = 0.0000000;
	freqtable[65] = 0.0077217;
	freqtable[66] = 0.0023391;
	freqtable[67] = 0.0031721;
	freqtable[68] = 0.0022720;
	freqtable[69] = 0.0060436;
	freqtable[70] = 0.0019232;
	freqtable[71] = 0.0017680;
	freqtable[72] = 0.0029635;
	freqtable[73] = 0.0091648;
	freqtable[74] = 0.0003476;
	freqtable[75] = 0.0009676;
	freqtable[76] = 0.0036323;
	freqtable[77] = 0.0025219;
	freqtable[78] = 0.0041701;
	freqtable[79] = 0.0045936;
	freqtable[80] = 0.0018437;
	freqtable[81] = 0.0002112;
	freqtable[82] = 0.0040014;
	freqtable[83] = 0.0052489;
	freqtable[84] = 0.0067908;
	freqtable[85] = 0.0020450;
	freqtable[86] = 0.0005564;
	freqtable[87] = 0.0029596;
	freqtable[88] = 0.0000599;
	freqtable[89] = 0.0012700;
	freqtable[90] = 0.0000972;
	freqtable[91] = 0.0007548;
	freqtable[92] = 0.0000000;
	freqtable[93] = 0.0007545;
	freqtable[94] = 0.0000000;
	freqtable[95] = 0.0017207;
	freqtable[96] = 0.0000000;
	freqtable[97] = 0.0476453;
	freqtable[98] = 0.0090749;
	freqtable[99] = 0.0132254;
	freqtable[100] = 0.0260751;
	freqtable[101] = 0.0804583;
	freqtable[102] = 0.0134101;
	freqtable[103] = 0.0112100;
	freqtable[104] = 0.0426480;
	freqtable[105] = 0.0390999;
	freqtable[106] = 0.0005441;
	freqtable[107] = 0.0057721;
	freqtable[108] = 0.0285880;
	freqtable[109] = 0.0184656;
	freqtable[110] = 0.0423756;
	freqtable[111] = 0.0546900;
	freqtable[112] = 0.0091430;
	freqtable[113] = 0.0005001;
	freqtable[114] = 0.0410141;
	freqtable[115] = 0.0424709;
	freqtable[116] = 0.0563976;
	freqtable[117] = 0.0224005;
	freqtable[118] = 0.0066340;
	freqtable[119] = 0.0141496;
	freqtable[120] = 0.0009033;
	freqtable[121] = 0.0164281;
	freqtable[122] = 0.0002213;
	freqtable[123] = 0.0000000;
	freqtable[124] = 0.0000000;
	freqtable[125] = 0.0000000;
	freqtable[126] = 0.0000000;
	freqtable[127] = 0.0000000;
	freqtable[128] = 0.0055947;
	freqtable[129] = 0.0000000;
	freqtable[130] = 0.0000000;
	freqtable[131] = 0.0000000;
	freqtable[132] = 0.0000101;
	freqtable[133] = 0.0000000;
	freqtable[134] = 0.0000011;
	freqtable[135] = 0.0000034;
	freqtable[136] = 0.0000000;
	freqtable[137] = 0.0000016;
	freqtable[138] = 0.0000000;
	freqtable[139] = 0.0000000;
	freqtable[140] = 0.0000000;
	freqtable[141] = 0.0000000;
	freqtable[142] = 0.0000000;
	freqtable[143] = 0.0000000;
	freqtable[144] = 0.0000000;
	freqtable[145] = 0.0000000;
	freqtable[146] = 0.0000000;
	freqtable[147] = 0.0000030;
	freqtable[148] = 0.0004443;
	freqtable[149] = 0.0000000;
	freqtable[150] = 0.0000000;
	freqtable[151] = 0.0000000;
	freqtable[152] = 0.0000662;
	freqtable[153] = 0.0046721;
	freqtable[154] = 0.0000000;
	freqtable[155] = 0.0000000;
	freqtable[156] = 0.0002204;
	freqtable[157] = 0.0001898;
	freqtable[158] = 0.0000000;
	freqtable[159] = 0.0000000;
	freqtable[160] = 0.0000025;
	freqtable[161] = 0.0000000;
	freqtable[162] = 0.0000112;
	freqtable[163] = 0.0000000;
	freqtable[164] = 0.0000000;
	freqtable[165] = 0.0000000;
	freqtable[166] = 0.0000121;
	freqtable[167] = 0.0000059;
	freqtable[168] = 0.0000067;
	freqtable[169] = 0.0000078;
	freqtable[170] = 0.0000011;
	freqtable[171] = 0.0000007;
	freqtable[172] = 0.0000000;
	freqtable[173] = 0.0000000;
	freqtable[174] = 0.0000004;
	freqtable[175] = 0.0000000;
	freqtable[176] = 0.0000000;
	freqtable[177] = 0.0000000;
	freqtable[178] = 0.0000000;
	freqtable[179] = 0.0000000;
	freqtable[180] = 0.0000000;
	freqtable[181] = 0.0000000;
	freqtable[182] = 0.0000000;
	freqtable[183] = 0.0000000;
	freqtable[184] = 0.0000000;
	freqtable[185] = 0.0000000;
	freqtable[186] = 0.0000000;
	freqtable[187] = 0.0000002;
	freqtable[188] = 0.0000000;
	freqtable[189] = 0.0000000;
	freqtable[190] = 0.0000000;
	freqtable[191] = 0.0000002;
	freqtable[192] = 0.0000000;
	freqtable[193] = 0.0000000;
	freqtable[194] = 0.0000000;
	freqtable[195] = 0.0000436;
	freqtable[196] = 0.0000000;
	freqtable[197] = 0.0000030;
	freqtable[198] = 0.0000000;
	freqtable[199] = 0.0000000;
	freqtable[200] = 0.0000000;
	freqtable[201] = 0.0000000;
	freqtable[202] = 0.0000000;
	freqtable[203] = 0.0000000;
	freqtable[204] = 0.0000000;
	freqtable[205] = 0.0000000;
	freqtable[206] = 0.0000000;
	freqtable[207] = 0.0000000;
	freqtable[208] = 0.0000000;
	freqtable[209] = 0.0000000;
	freqtable[210] = 0.0000000;
	freqtable[211] = 0.0000000;
	freqtable[212] = 0.0000000;
	freqtable[213] = 0.0000000;
	freqtable[214] = 0.0000000;
	freqtable[215] = 0.0000000;
	freqtable[216] = 0.0000000;
	freqtable[217] = 0.0000000;
	freqtable[218] = 0.0000000;
	freqtable[219] = 0.0000000;
	freqtable[220] = 0.0000000;
	freqtable[221] = 0.0000000;
	freqtable[222] = 0.0000000;
	freqtable[223] = 0.0000000;
	freqtable[224] = 0.0000000;
	freqtable[225] = 0.0000000;
	freqtable[226] = 0.0056041;
	freqtable[227] = 0.0000000;
	freqtable[228] = 0.0000000;
	freqtable[229] = 0.0000000;
	freqtable[230] = 0.0000000;
	freqtable[231] = 0.0000000;
	freqtable[232] = 0.0000000;
	freqtable[233] = 0.0000000;
	freqtable[234] = 0.0000000;
	freqtable[235] = 0.0000000;
	freqtable[236] = 0.0000000;
	freqtable[237] = 0.0000000;
	freqtable[238] = 0.0000000;
	freqtable[239] = 0.0000002;
	freqtable[240] = 0.0000000;
	freqtable[241] = 0.0000000;
	freqtable[242] = 0.0000000;
	freqtable[243] = 0.0000000;
	freqtable[244] = 0.0000000;
	freqtable[245] = 0.0000000;
	freqtable[246] = 0.0000000;
	freqtable[247] = 0.0000000;
	freqtable[248] = 0.0000000;
	freqtable[249] = 0.0000000;
	freqtable[250] = 0.0000000;
	freqtable[251] = 0.0000000;
	freqtable[252] = 0.0000000;
	freqtable[253] = 0.0000000;
	freqtable[254] = 0.0000000;
	freqtable[255] = 0.0000000;

	double score = 0;

	for ( size_t i = 0 ; i < rawlen ; i++ )
	{
		score += freqtable[raw[i]];
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

	double current_score = 0, max_score = 0;

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

		if ( current_score > max_score )
		{
			max_score = current_score;

			memset(best_candidate,0x00,ctlen * sizeof(uint8_t));

			memcpy(best_candidate,candidate,ctlen);
		}	
		
		printf("\nscore:%.9lf\ncandidate str:\n%s\n\n",current_score,candidate);

	}
		
	printf("\nscore:%.9lf\nbest_candidate:\n%s\n\n",max_score,best_candidate);
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
