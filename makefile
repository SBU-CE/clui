
run: compile
	./a.exe

compile:
	gcc -Wall sample.c -o a.exe

clean:
	rm *.exe
