# SPDX-License-Identifier: GPL-2.0-or-later
head := rbtree.h macro.h
obj := demo.o rbtree.o

demo: $(obj)
	@ echo -e "  \e[34mMKELF\e[0m	" $@
	@ gcc -o $@ $(obj)

clean:
	@ rm $(obj) demo

%.o:%.c $(head)
	@ echo -e "  \e[32mCC\e[0m	" $@
	@ gcc -o $@ -c $< -g -Wall -Werror # -Wextra
