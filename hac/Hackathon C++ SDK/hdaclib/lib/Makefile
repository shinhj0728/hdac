CC	= g++ -O
CFLAGS	= 

all:	hashlib.a test

hashlib.a: md5.o rc4.o sha1.o sha256.o sha512.o base58.o libhash.o
	rm -f $@
	ar cru $@ md5.o rc4.o sha1.o sha256.o sha512.o base58.o libhash.o

test: test_hashlib test_base58 test_md5 test_rc4 test_sha1 test_sha256 test_sha512

test_hashlib: test_hashlib.o hashlib.a
	$(CC) $@.c -o $@ hashlib.a

test_base58: test_base58.o hashlib.a
	$(CC) $@.c -o $@ hashlib.a

test_md5: test_md5.o hashlib.a
	$(CC) $@.c -o $@ hashlib.a

test_rc4: test_rc4.o hashlib.a
	$(CC) $@.c -o $@ hashlib.a

test_sha1: test_sha1.o hashlib.a
	$(CC) $@.c -o $@ hashlib.a

test_sha256: test_sha256.o hashlib.a
	$(CC) $@.c -o $@ hashlib.a

test_sha512: test_sha512.o hashlib.a
	$(CC) $@.c -o $@ hashlib.a


clean:
	rm -f *.o hashlib.a
