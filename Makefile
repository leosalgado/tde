all:
	gcc main.c -o main -Wall

run:
	gcc main.c -o main -Wall && ./main

clean:
	rm main out.csv
