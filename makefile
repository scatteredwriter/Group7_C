all:
	arm-linux-gcc -o ./Bin/group7 ./Source/*.c -I ./Include/ -I /usr/arm-2013.05/arm-none-linux-gnueabi/include/freetype2/ -L ./Library -l ts -l pthread -l freetype -l m