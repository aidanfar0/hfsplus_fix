CC=gcc

hfsplus_fix: hfsplus_fix.o pretty.o
	$(CC) -o hfsplus_fix hfsplus_fix.o pretty.o $(CFLAGS)

clean:
	rm -f *.o hfsplus_fix
