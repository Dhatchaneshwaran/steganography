a.out : project_main.o encode.o decode.o
	gcc -o a.out project_main.o encode.o decode.o
project_main.o : project_main.c
	gcc -c project_main.c
encode.o : encode.c
	gcc -c encode.c
decode.o : decode.c
	gcc -c decode.c
clean :
	rm *.out *.o
