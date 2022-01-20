# SPDX-License-Identifier: GPL-2.0-or-later
head := rbtree.h
demo := benchmark simple
obj := rbtree.o benchmark.o simple.o

all: $(demo)

%.o:%.c $(head)
	@ echo -e "  \e[32mCC\e[0m	" $@
	@ gcc -o $@ -c $< -g -O2 -Wall -Werror -Wextra

$(demo): $(obj)
	@ echo -e "  \e[34mMKELF\e[0m	" $@
	@ gcc -o $@ $@.c $< -g -O2 -Wall -Werror -Wextra

clean:
	@ rm -f $(obj) $(demo)
