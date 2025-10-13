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
                                                     -Wl,--as-needed -Wl,--no-copy-dt-needed-entries set1.c -o /tmp/set1.o

/tmp/set1.o 49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d

echo "Valgrind output below:"

echo ""

valgrind /tmp/set1.o 49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d


