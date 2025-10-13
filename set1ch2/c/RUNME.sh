#!/usr/bin/bash
'''
https://github.com/ossf/wg-best-practices-os-developers/blob/main/docs/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C%2B%2B.md
'''

gcc -O2 -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough \
                                                     -Werror=format-security \
                                                     -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
                                                     -D_GLIBCXX_ASSERTIONS \
                                                     -fstack-clash-protection -fstack-protector-strong \
                                                     -Wl,-z,nodlopen -Wl,-z,noexecstack \
                                                     -Wl,-z,relro -Wl,-z,now \
                                                     -Wl,--as-needed
-Wl,--no-copy-dt-needed-entries set1ch2.c -o /tmp/set1ch2.o

/tmp/set1ch2.o 1c0111001f010100061a024b53535009181c 686974207468652062756c6c277320657965 

echo "Valgrind output below:"

echo ""

valgrind /tmp/set1ch2.o 1c0111001f010100061a024b53535009181c 686974207468652062756c6c277320657965 
