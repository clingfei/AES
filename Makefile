AES: AES.o
	cc -o AES AES.o
AES.o: AES.c AES.h
	cc -c AES.c
clean:
	-rm *.o AES