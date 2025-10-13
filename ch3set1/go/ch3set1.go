/*
Lesson learned: It is still possible to make pointer

reference mistakes in Golang!

If one initializes a variable as a pointer to an array you

must make sure the pointer points to an object that has a desirable

state when information is printed

*/

package main

import	(
	"fmt"
	"os"
	"log"
	"encoding/hex"
)

func score(input []byte)	float64	{
	
	var freqtable [256]float64

	for i := 0 ; i < 256 ; i++	{
	
		freqtable[i] = 0.0
	}		

	freqtable[0] = 0.0000000
	freqtable[1] = 0.0000000
	freqtable[2] = 0.0000000
	freqtable[3] = 0.0000000
	freqtable[4] = 0.0000000
	freqtable[5] = 0.0000000
	freqtable[6] = 0.0000000
	freqtable[7] = 0.0000000
	freqtable[8] = 0.0000000
	freqtable[9] = 0.0000007
	freqtable[10] = 0.0348309
	freqtable[11] = 0.0000000
	freqtable[12] = 0.0000000
	freqtable[13] = 0.0348309
	freqtable[14] = 0.0000000
	freqtable[15] = 0.0000000
	freqtable[16] = 0.0000000
	freqtable[17] = 0.0000000
	freqtable[18] = 0.0000000
	freqtable[19] = 0.0000000
	freqtable[20] = 0.0000000
	freqtable[21] = 0.0000000
	freqtable[22] = 0.0000000
	freqtable[23] = 0.0000000
	freqtable[24] = 0.0000000
	freqtable[25] = 0.0000000
	freqtable[26] = 0.0000000
	freqtable[27] = 0.0000000
	freqtable[28] = 0.0000000
	freqtable[29] = 0.0000000
	freqtable[30] = 0.0000000
	freqtable[31] = 0.0000000
	freqtable[32] = 0.1476086
	freqtable[33] = 0.0013844
	freqtable[34] = 0.0000000
	freqtable[35] = 0.0000002
	freqtable[36] = 0.0000004
	freqtable[37] = 0.0000002
	freqtable[38] = 0.0000085
	freqtable[39] = 0.0000002
	freqtable[40] = 0.0000415
	freqtable[41] = 0.0000415
	freqtable[42] = 0.0000032
	freqtable[43] = 0.0000000
	freqtable[44] = 0.0167989
	freqtable[45] = 0.0006929
	freqtable[46] = 0.0162071
	freqtable[47] = 0.0000011
	freqtable[48] = 0.0000275
	freqtable[49] = 0.0000548
	freqtable[50] = 0.0000323
	freqtable[51] = 0.0000192
	freqtable[52] = 0.0000278
	freqtable[53] = 0.0000176
	freqtable[54] = 0.0000261
	freqtable[55] = 0.0000144
	freqtable[56] = 0.0000262
	freqtable[57] = 0.0000163
	freqtable[58] = 0.0006078
	freqtable[59] = 0.0022914
	freqtable[60] = 0.0000000
	freqtable[61] = 0.0000000
	freqtable[62] = 0.0000000
	freqtable[63] = 0.0020211
	freqtable[64] = 0.0000000
	freqtable[65] = 0.0077217
	freqtable[66] = 0.0023391
	freqtable[67] = 0.0031721
	freqtable[68] = 0.0022720
	freqtable[69] = 0.0060436
	freqtable[70] = 0.0019232
	freqtable[71] = 0.0017680
	freqtable[72] = 0.0029635
	freqtable[73] = 0.0091648
	freqtable[74] = 0.0003476
	freqtable[75] = 0.0009676
	freqtable[76] = 0.0036323
	freqtable[77] = 0.0025219
	freqtable[78] = 0.0041701
	freqtable[79] = 0.0045936
	freqtable[80] = 0.0018437
	freqtable[81] = 0.0002112
	freqtable[82] = 0.0040014
	freqtable[83] = 0.0052489
	freqtable[84] = 0.0067908
	freqtable[85] = 0.0020450
	freqtable[86] = 0.0005564
	freqtable[87] = 0.0029596
	freqtable[88] = 0.0000599
	freqtable[89] = 0.0012700
	freqtable[90] = 0.0000972
	freqtable[91] = 0.0007548
	freqtable[92] = 0.0000000
	freqtable[93] = 0.0007545
	freqtable[94] = 0.0000000
	freqtable[95] = 0.0017207
	freqtable[96] = 0.0000000
	freqtable[97] = 0.0476453
	freqtable[98] = 0.0090749
	freqtable[99] = 0.0132254
	freqtable[100] = 0.0260751
	freqtable[101] = 0.0804583
	freqtable[102] = 0.0134101
	freqtable[103] = 0.0112100
	freqtable[104] = 0.0426480
	freqtable[105] = 0.0390999
	freqtable[106] = 0.0005441
	freqtable[107] = 0.0057721
	freqtable[108] = 0.0285880
	freqtable[109] = 0.0184656
	freqtable[110] = 0.0423756
	freqtable[111] = 0.0546900
	freqtable[112] = 0.0091430
	freqtable[113] = 0.0005001
	freqtable[114] = 0.0410141
	freqtable[115] = 0.0424709
	freqtable[116] = 0.0563976
	freqtable[117] = 0.0224005
	freqtable[118] = 0.0066340
	freqtable[119] = 0.0141496
	freqtable[120] = 0.0009033
	freqtable[121] = 0.0164281
	freqtable[122] = 0.0002213
	freqtable[123] = 0.0000000
	freqtable[124] = 0.0000000
	freqtable[125] = 0.0000000
	freqtable[126] = 0.0000000
	freqtable[127] = 0.0000000
	freqtable[128] = 0.0055947
	freqtable[129] = 0.0000000
	freqtable[130] = 0.0000000
	freqtable[131] = 0.0000000
	freqtable[132] = 0.0000101
	freqtable[133] = 0.0000000
	freqtable[134] = 0.0000011
	freqtable[135] = 0.0000034
	freqtable[136] = 0.0000000
	freqtable[137] = 0.0000016
	freqtable[138] = 0.0000000
	freqtable[139] = 0.0000000
	freqtable[140] = 0.0000000
	freqtable[141] = 0.0000000
	freqtable[142] = 0.0000000
	freqtable[143] = 0.0000000
	freqtable[144] = 0.0000000
	freqtable[145] = 0.0000000
	freqtable[146] = 0.0000000
	freqtable[147] = 0.0000030
	freqtable[148] = 0.0004443
	freqtable[149] = 0.0000000
	freqtable[150] = 0.0000000
	freqtable[151] = 0.0000000
	freqtable[152] = 0.0000662
	freqtable[153] = 0.0046721
	freqtable[154] = 0.0000000
	freqtable[155] = 0.0000000
	freqtable[156] = 0.0002204
	freqtable[157] = 0.0001898
	freqtable[158] = 0.0000000
	freqtable[159] = 0.0000000
	freqtable[160] = 0.0000025
	freqtable[161] = 0.0000000
	freqtable[162] = 0.0000112
	freqtable[163] = 0.0000000
	freqtable[164] = 0.0000000
	freqtable[165] = 0.0000000
	freqtable[166] = 0.0000121
	freqtable[167] = 0.0000059
	freqtable[168] = 0.0000067
	freqtable[169] = 0.0000078
	freqtable[170] = 0.0000011
	freqtable[171] = 0.0000007
	freqtable[172] = 0.0000000
	freqtable[173] = 0.0000000
	freqtable[174] = 0.0000004
	freqtable[175] = 0.0000000
	freqtable[176] = 0.0000000
	freqtable[177] = 0.0000000
	freqtable[178] = 0.0000000
	freqtable[179] = 0.0000000
	freqtable[180] = 0.0000000
	freqtable[181] = 0.0000000
	freqtable[182] = 0.0000000
	freqtable[183] = 0.0000000
	freqtable[184] = 0.0000000
	freqtable[185] = 0.0000000
	freqtable[186] = 0.0000000
	freqtable[187] = 0.0000002
	freqtable[188] = 0.0000000
	freqtable[189] = 0.0000000
	freqtable[190] = 0.0000000
	freqtable[191] = 0.0000002
	freqtable[192] = 0.0000000
	freqtable[193] = 0.0000000
	freqtable[194] = 0.0000000
	freqtable[195] = 0.0000436
	freqtable[196] = 0.0000000
	freqtable[197] = 0.0000030
	freqtable[198] = 0.0000000
	freqtable[199] = 0.0000000
	freqtable[200] = 0.0000000
	freqtable[201] = 0.0000000
	freqtable[202] = 0.0000000
	freqtable[203] = 0.0000000
	freqtable[204] = 0.0000000
	freqtable[205] = 0.0000000
	freqtable[206] = 0.0000000
	freqtable[207] = 0.0000000
	freqtable[208] = 0.0000000
	freqtable[209] = 0.0000000
	freqtable[210] = 0.0000000
	freqtable[211] = 0.0000000
	freqtable[212] = 0.0000000
	freqtable[213] = 0.0000000
	freqtable[214] = 0.0000000
	freqtable[215] = 0.0000000
	freqtable[216] = 0.0000000
	freqtable[217] = 0.0000000
	freqtable[218] = 0.0000000
	freqtable[219] = 0.0000000
	freqtable[220] = 0.0000000
	freqtable[221] = 0.0000000
	freqtable[222] = 0.0000000
	freqtable[223] = 0.0000000
	freqtable[224] = 0.0000000
	freqtable[225] = 0.0000000
	freqtable[226] = 0.0056041
	freqtable[227] = 0.0000000
	freqtable[228] = 0.0000000
	freqtable[229] = 0.0000000
	freqtable[230] = 0.0000000
	freqtable[231] = 0.0000000
	freqtable[232] = 0.0000000
	freqtable[233] = 0.0000000
	freqtable[234] = 0.0000000
	freqtable[235] = 0.0000000
	freqtable[236] = 0.0000000
	freqtable[237] = 0.0000000
	freqtable[238] = 0.0000000
	freqtable[239] = 0.0000002
	freqtable[240] = 0.0000000
	freqtable[241] = 0.0000000
	freqtable[242] = 0.0000000
	freqtable[243] = 0.0000000
	freqtable[244] = 0.0000000
	freqtable[245] = 0.0000000
	freqtable[246] = 0.0000000
	freqtable[247] = 0.0000000
	freqtable[248] = 0.0000000
	freqtable[249] = 0.0000000
	freqtable[250] = 0.0000000
	freqtable[251] = 0.0000000
	freqtable[252] = 0.0000000
	freqtable[253] = 0.0000000
	freqtable[254] = 0.0000000
	freqtable[255] = 0.0000000

	var score float64 = 0.0
	
	for i := 0 ; i < len(input) ; i++	{
	
		score += freqtable[input[i]]	
	}

	return score
}

func decrypt(input []byte)	[]byte	{

	var key byte = 0x00

	var current_score float64 = 0.0

	var max_score float64 = 0.0

	var best_text []byte = make([]byte,len(input))

	var ciphertext = make([]byte,len(input))

	for i := 0 ; i < len(ciphertext) ; i++	{
		
		ciphertext[i] = 0x00
	}

	for i := 0 ; i <= 0xff ; i++	{
		
		for j := 0 ; j < len(input) ; j++	{
			
			ciphertext[j] = 0x00
		}	

		for j := 0 ; j < len(input) ; j++	{
		
			ciphertext[j] = input[j] ^ key
		}
		
		current_score = score(ciphertext)

		fmt.Printf("current_score: %.15g\n",current_score)

		fmt.Printf("ciphertext: %s\n",ciphertext)

		if current_score > max_score	{
		
			max_score = current_score

			for i := 0 ; i < len(ciphertext) ; i++	{
				
				best_text[i] = ciphertext[i]
			}

		}	
		
		key++		
	}

	fmt.Printf("max score: %.15g\n",max_score)

	return best_text

}

func main()	{
	
	input := os.Args[1]

	input_hex, err := hex.DecodeString(input)

	if err != nil	{
		
		log.Fatal(err)
	}

	var plaintext []byte = decrypt(input_hex)

	fmt.Printf("best candidate: %s\n",string(plaintext[:]))	
}
