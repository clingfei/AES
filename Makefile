AES: AES.o
	cc -o AES AES.o
AES.o: AES.c
	cc -c AES.c
clean:
	-rm *.o AES