all : ssu_sdup ssu_find-md5 ssu_help find-sha1
ssu_sdup : ssu_sdup.c
	gcc -o ssu_sdup ssu_sdup.c

ssu_find-md5 : ssu_find-md5.c
	gcc -o ssu_find-md5 ssu_find-md5.c -lcrypto

find-sha1 : find-sha1.c
	gcc -o find-sha1 find-sha1.c -lcrypto

ssu_help : ssu_help.c
	gcc -o ssu_help ssu_help.c

