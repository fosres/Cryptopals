package main

import	(
	"fmt"
	"os"
	"log"
	"encoding/base64"
	"encoding/hex"
)

func main()	{
	
	data, err := hex.DecodeString(os.Args[1])

	if err != nil	{
		
		log.Fatal(err)
	}

	base64data := base64.StdEncoding.EncodeToString(data)

	fmt.Println(base64data)
}
