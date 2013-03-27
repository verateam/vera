build :
	make -C src
	mv src/vera++ .

install :
	install vera++ $(DESTDIR)/usr/bin

clean :
	make -C src clean
	rm -f vera++
