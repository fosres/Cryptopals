#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

int main(size_t argc, unsigned char * argv[])
{
	if ( argc != 2 )
	{
		fprintf(stderr,"Error: Incorrect # args. Aborting\n");

		exit(1);
	}

	FILE * corpus = NULL;

	if ( ( corpus = fopen(argv[1],"r") ) == NULL )
	{
		fprintf(stderr,"Error: Failed to open corpus!\n");

		exit(1);	
	}

	fseek(corpus, 0L, SEEK_END);

	size_t corpus_size = ftell(corpus);

	printf("corpus_size: %zu\n",corpus_size);

	rewind(corpus);

	uint8_t c = 0;

	double freqtable[256];

	for ( size_t i = 0; i < 256 ; i++ )
	{
		freqtable[i] = 0.0;
	}

	size_t i = 0;
	
	while ( ( c = fgetc(corpus) ) != 0xff )
	{
		freqtable[c] += 1.0;				

		i++;
	}

	printf("\ni: %zu\n",i);

	rewind(corpus);

	for ( size_t index = 0 ; index < 256 ; index++ )
	{
		freqtable[index] /= (double)corpus_size;

		if ( freqtable[index] - 0.0 < 1.0e-9 )
		{
			freqtable[index] = 1.0e-9;
		}
	}

	for ( size_t index = 0 ; index < 256 ; index++ )
	{
		printf("freqtable[%zu] = %.9lf;\n",index,freqtable[index]);
	}

	fclose(corpus);
	
	return 0;
}
