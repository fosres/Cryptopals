#!/usr/bin/bash

: '
https://github.com/ossf/wg-best-practices-os-developers/blob/main/docs/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C%2B%2B.md

'

gcc -O2 -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough \
                                                     -Werror=format-security \
                                                     -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
                                                     -D_GLIBCXX_ASSERTIONS \
                                                     -fstack-clash-protection -fstack-protector-strong \
                                                     -Wl,-z,nodlopen -Wl,-z,noexecstack \
                                                     -Wl,-z,relro -Wl,-z,now \
                                                     -Wl,--as-needed \
-Wl,--no-copy-dt-needed-entries ch4.c -o /tmp/ch4.o

/tmp/ch4.o 1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736

echo "Valgrind output below:"

echo ""

valgrind /tmp/ch4.o 1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736


