fat: fat.c recursive.c print.c
	gcc $^ -o $@ 

clean:
	rm fat
