use std::env;
use std::process;
use std::str;
use hex::decode;
//use base64::encode;

use is_printable::IsPrintable;

fn	decrypt(rawarr: Vec<u8>)
{
	let mut plaintext = Vec::new();

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

		if  plaintext.len() > 0
		{

			let plaintext_str : String = 

			plaintext.into_iter().collect()
			
			;
			
			println!("plaintext: {:?}\n",plaintext_str); 
		}

		plaintext = Vec::new();		

	}	
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

	decrypt(raw_arr);
}
