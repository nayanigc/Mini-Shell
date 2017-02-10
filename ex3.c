#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


int simple_cmd(char *argv[]){
	int a = 0;
	while (argv[a]!= NULL ){
		if (strpbrk(argv[a],"exit")){
			exit (1);
		}else{
			printf ("%s",argv[a]);
			a++;
		}
	}
	return 0;

}

int parse_line(char* s){
	int a =0;
	char argv[100];
	while(strcmp (s[a],'\0')){
		if (strcmp(s," ")){
			 argv[a] = s[a] ;
			a++;
		}
	}

	int b = simple_cmd(argv);
	return a;

}

int main (){
	//char* t = fgets();
	int r = parse_line("toto");
	return 0;
}

