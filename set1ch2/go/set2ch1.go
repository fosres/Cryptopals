package main

import	(
	"fmt"
	"log"
	"os"
	"errors"
	"encoding/hex"
)

func main()	{

	fst_hex := os.Args[1] 

	snd_hex := os.Args[2] 

	if len(fst_hex) != len(snd_hex)	{

		log.Fatal(errors.New("hex strings not equal"))	
	}

	fst, err1 := hex.DecodeString(fst_hex)

	if err1 != nil	{
		
		log.Fatal(err1)
	}

	snd, err2 := hex.DecodeString(snd_hex)
	
	if err2 != nil	{
		
		log.Fatal(err2)
	}


	xor_combo := make([]byte,len(fst))
	
	for i := 0 ; i	< len(fst) ; i++	{
		
		xor_combo[i] = fst[i] ^ snd[i]	
	}

	xor_combo_hex := hex.EncodeToString(xor_combo)

	fmt.Println(xor_combo_hex)

}
