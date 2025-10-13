use std::env;
use std::process;
use hex::decode;
use base64::encode;


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

	println!("raw_arr: {:?}",raw_arr);

	let base64str = base64::encode(raw_arr);

	println!("base64str: {:?}",base64str);

}
