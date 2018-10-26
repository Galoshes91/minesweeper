all:

	gcc server.c game.c -o server -std=c99
	gcc client.c -o client -std=c99

clean:
	rm server
	rm client
