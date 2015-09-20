main: 
	gcc -std=c99 -o pi pi.c
	gcc -mavx2 -std=c99 -o piSIMD piSIMD.c

clean:
	rm -f pi piSIMD
