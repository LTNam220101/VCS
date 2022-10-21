#include  <stdio.h> 

void in(char* str){
	// This line is vulnerable
	printf(str);
}

void in2(char* str){
	// This line is safe
	printf("%s\n", str);
}


//./fs "Hello World %p%p"
void main(int argc, char **argv){
    in(argv[1]);
    in2(argv[1]);
}