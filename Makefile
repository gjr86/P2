# compiling

spell: clean
	gcc -Wall -Werror -fsanitize=address,undefined -g spell.c -o spell

clean:
	rm -f spell

debug:
	gcc -Wall -Werror -fsanitize=address,undefined -g -DDEBUG=1 spell.c -o spell



# testing

test1:
	./spell dict.txt test1.txt