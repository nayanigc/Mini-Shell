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
	int b= 0;
	char* argv[100];
	while(strpbrk (s,"\0")== NULL ){
		int compteur =0;
		if (strpbrk(argv[a]," ")== NULL){
			 argv[a] = (char *) s;
			compteur++;
			
		}
		while(b != compteur){
			printf("%s",argv[b]);
			b++;
		
		}
		a++;	
		
	}

	int c = simple_cmd(argv);
	return c;

}

int main (int argc, char** argv){

	char* t = "command arg1 arg2 #arg3 exit" ;
	/*while ( fgets(t,5,0) != NULL){	
		printf("$");
	}*/
	int r = parse_line(t);
	printf("%d",r);
	return 0;

}

