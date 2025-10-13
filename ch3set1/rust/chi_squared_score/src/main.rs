use std::env;
use std::process;
use std::str;
use hex::decode;
//use base64::encode;

use is_printable::IsPrintable;

fn	score(rawarr: Vec<char>) -> f64
{
	let mut freqtable: [f64 ; 256] = [ 0.0 ; 256 ];

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
	freqtable[10] = 0.000000001; // treat newline as insignificant
	//freqtable[10] = 0.034830918;
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

	let mut freqtable_doc : [f64 ; 256] = [0.0 ; 256];	

	let rawarr_len : f64 = rawarr.len() as f64;

	for i in 0..rawarr.len()		{

		let char_usize = rawarr[i] as usize;
		
		freqtable_doc[char_usize] += 1.0;
	}

	for i in 0..=255	{
		
		freqtable_doc[i] /= rawarr_len;
	}

	let mut chi_score : f64 = 0.0f64;
	
	let mut diff : f64 = 0.0f64;

	let rawarr_len = rawarr.len() as f64;

	for i in 0..=255	{
		
		diff = freqtable_doc[i] - freqtable[i];
	
		chi_score += ( ( diff * diff ) / rawarr_len );
	}

	return chi_score;
}

fn	decrypt(rawarr: Vec<u8>) -> Vec<char>
{
	let mut plaintext = Vec::new();

	let mut best_text = Vec::new();

	let mut current_score : f64 = 0.0f64;
	
	let mut min_score : f64 = f64::MAX;

	for key in 0..=255
	{
		for i in 0..rawarr.len()
		{
			let ch = rawarr[i] ^ key;

			let ch_char = ch as char;

			if ch_char.is_printable() == false
			{
				plaintext = Vec::new();		

				break;
			}

			else
			{
				plaintext.push(ch_char);
			}

	
		//	print!("{}",&rawarr[i]);			
		}

		current_score = score(plaintext.clone());

		if current_score < min_score
		{
			min_score = current_score;

			best_text = plaintext.clone();
		}

		
		if  plaintext.len() > 0
		{

			let plaintext_str : String = 

			plaintext.into_iter().collect()
			
			;
			
			println!("plaintext: {:?}\n",plaintext_str); 
		}

		plaintext = Vec::new();		
	}

	return best_text;	
}

/*
Rust still faces issues with invalid or missing arguments

watch out for these in your code. It can cause your code to

malfunction.

https://hatchjs.com/rust-parse-hex-string/

*/

fn	strtohex(str: String) -> Vec<u8>
{
	return hex::decode(str).unwrap();
}

fn	main()
{
	
	let args : Vec<String> = env::args().collect();
	
	let argc = args.len();

	if argc < 2
	{
		eprintln!("Error: Argument for hex string missing!");

		process::abort();
	}

	let hexstr = &args[1].clone();
	
	let endn = 

	if hexstr.len() > 1000000
	{
		1000000
	}
	
	else
	{
		hexstr.len()	
	};
	
	let hexstr = &hexstr[0..endn];
	
	if hexstr.len() % 2 != 0
	{
		eprintln!("Error: hex string is not even length of");
		
		eprintln!("digits. Aborting!\n");
		
		process::abort();
	}

	let raw_arr = strtohex(hexstr.to_string());

//	println!("raw_arr: {:?}",raw_arr);

	let plaintext_vec_char = decrypt(raw_arr);

	let plaintext = plaintext_vec_char.iter().collect::<String>();

	println!("Plaintext: {:?}",plaintext);

}
