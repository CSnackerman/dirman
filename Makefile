all:
	gcc main.c -o dirman

clean:
	rm -f dirman && rm -rf -- */

clean-dirs:
	rm -rf -- */

install:
	gcc main.c -o dirman && cp ./dirman /usr/local/bin/dirman

uninstall:
	rm -f /usr/local/bin/dirman