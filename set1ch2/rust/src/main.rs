use std::env;
use std::process;
use hex::encode;
use hex::decode;


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

	if argc != 3
	{
		eprintln!("Error: Wrong number of arguments!");

		process::abort();
	}

	if args[1].len() != args[2].len()
	{
		eprintln!("Error: Hex strings have different length.");

		process::abort();
	}

	let arg1str = &args[1].clone();
	
	let endn = 

	if arg1str.len() > 1000000
	{
		1000000
	}
	
	else
	{
		arg1str.len()	
	};
	
	let arg1str = &arg1str[0..endn];
	
	if arg1str.len() % 2 != 0
	{
		eprintln!("Error: hex string is not even length of");
		
		eprintln!("digits. Aborting!\n");
		
		process::abort();
	}

	let raw1 = strtohex(arg1str.to_string());

	println!("raw1: {:?}",raw1);

	let arg2str = &args[2].clone();
	
	let endn2 = 

	if arg2str.len() > 1000000
	{
		1000000
	}
	
	else
	{
		arg2str.len()	
	};

	let arg2str = &arg2str[0..endn2];
	
	if arg2str.len() % 2 != 0
	{
		eprintln!("Error: hex string is not even length of");
		
		eprintln!("digits. Aborting!\n");
		
		process::abort();
	}
	
	let raw2 = strtohex(arg2str.to_string());
	
	println!("raw2: {:?}",raw2);

	let mut xorarr: Vec<u8> = Vec::new();	

	for i in 0..raw1.len()
	{
		xorarr.push(raw1[i] ^ raw2[i]);
	}

	println!("xorarr: {:?}",xorarr);
	
	let xorhexstr = hex::encode(xorarr);

	println!("xorhexstr: {:?}",xorhexstr);	
}
