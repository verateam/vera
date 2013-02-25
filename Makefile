build :
	make -C src
	mv src/vera++ .

clean :
	make -C src clean
	rm -f vera++
