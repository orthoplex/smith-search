smith.out: smith.c
	gcc smith.c -O3 -lgmp -o smith.out
clean:
	rm smith.out
