#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int simple_cmd(char *argv[]){
	int a = 0;
	printf("*** simple_cmd *** \n");
	while (argv[a]!= NULL ){
		 if (strpbrk(argv[a],"#")){
			break;
			
		}else if ( strncmp (argv[a],"exit",4)==0){
			exit(1);
		}else{
			printf("%s\n",argv[a]);
			a++;
		}
	
	}


	pid_t pid;
	int status;
	
	if((pid = fork()) == 0){
		execvp(argv[0], argv);
		perror("execvp");
		return EXIT_FAILURE;
	} else if(pid > 0){
		wait(&status);
	} else {
		printf("Erreur");
		exit(1);
		
	}	

	return 0;

}

int parse_line(char* s){
	char* argv[100];
	char* word;
	argv[0]=s;
	int compteur = 1;
	while((word = strpbrk (s," ")) != NULL ){
			printf("espace détecté : %d\n",compteur);
			*word = '\0';
			s = word + 1;
			argv[compteur] = (char *) s;
			printf("argv[%d] = %s\n",compteur,s);
			printf("argv : %s\n", argv[compteur]);
			compteur++;
	}
	argv[compteur] = (char*) s;
	argv[compteur++] = NULL;

	for(int i=0; i<compteur;i++){
		printf("argv[%d]= %s \n",i,argv[i]);
	}
	int b = simple_cmd(argv);
	return b;

}



int main (int argc, char** argv){
	char t[50];
	do {	
		printf("$");
		fgets(t, sizeof(t), stdin);
		 parse_line(t);
	} while(1);

	return 0;

}
