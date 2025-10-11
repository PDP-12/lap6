all: lap6 convert extract check

lap6: lap6.py

convert: convert.c
	$(CC) $^ -o $@

extract: extract.c
	$(CC) $^ -o $@

check: lap6.py test/test.sh test/test.oct
	test/test.sh
