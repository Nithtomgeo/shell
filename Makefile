unishell: shell.c myls.c
	   gcc shell.c -o unishell
	   gcc myls.c -o myls
clean:
	rm -rf *o unishell
	rm -rf *o myls
