all: src/*
	gcc -O2 -o gsp_cpu src/*.c 
clean:
	$(RM) gsp *.o *~
