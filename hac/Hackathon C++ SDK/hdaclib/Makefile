CC		= g++
CFLAGS		= #-Ilib -O -std=c++11 
CPPFLAGS	= -Ilib -O -std=c++11 -Wwrite-strings

all:	test1 test dumphdac 

test1:	test1.o hdaclib.a hashlib.a jsonlib2.a
	$(CC) $(CPPFLAGS) test1.c -o test1 hdaclib.a hashlib.a jsonlib2.a -lcurl -lcrypto -lpthread 

test:	test.o hdaclib.a hashlib.a jsonlib2.a
	$(CC) $(CPPFLAGS) test.c -o test hdaclib.a hashlib.a jsonlib2.a -lcurl -lcrypto -lpthread 

dumphdac: dumphdac.o
	$(CC) $(CPPFLAGS) dumphdac.c -o dumphdac hdaclib.a hashlib.a jsonlib2.a -lcurl -lcrypto -lpthread 

clean:
	cd lib; make clean; cd ..
	rm -f *.o test1 test dumphdac 
