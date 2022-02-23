# SPDX-License-Identifier: GPL-2.0-or-later
flags = -g -O2 -Wall -Werror -Wextra
head  = rbtree.h
demo  = benchmark simple
obj   = rbtree.o

all: $(demo)

%.o:%.c $(head)
	@ echo -e "  \e[32mCC\e[0m	" $@
	@ gcc -o $@ -c $< $(flags)

$(demo): $(obj)
	@ echo -e "  \e[34mMKELF\e[0m	" $@
	@ gcc -o $@ $@.c $<  $(flags)

clean:
	@ rm -f $(obj) $(demo)
