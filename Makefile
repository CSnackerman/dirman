all:
	gcc main.c -o dirman

clean:
	rm -f dirman && rm -rf -- */

clean-dirs:
	rm -rf -- */