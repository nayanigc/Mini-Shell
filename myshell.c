#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUFSIZE 2048 
#define MAXARGS 100
#define SORTIE_STANDART ""

int simple_cmd(char *argv[]){
	
	//printf("*** simple_cmd *** \n");

	if ( strncmp (argv[0],"exit",4)==0){
			exit(1);
	}else if (strncmp(argv[0],"cd",2) == 0){
		 	chdir(argv[1]); 
			return 0;
	} else {
	//printf("******help\n********");
		pid_t pid;
		int status=0;
	
		if((pid = fork()) == 0){
			execvp(*argv, argv);
			perror("execvp");
			exit(1);
		} else if(pid > 0){
			wait(&status);
		} else {
			printf("Erreur");
			exit(1);
		
		}

	}
	
	return 0;

}

int redir_cmd(char* argv[], char* in, char* out){
	int fdin = (strcmp(in, SORTIE_STANDART) != 0) ? 0 : open(in, O_RDONLY);
	if(fdin < 0){
		perror(fdin);
	}
	int fdout = (strcmp(out, SORTIE_STANDART) != 0) ? 0 : open(in, O_RDONLY);
	if(fdout < 0){
		perror(fdout);
	}
	if(dup2(fdin, fdout) < 0){
		perror("dup2");
		return EXIT_FAILURE;
	}
	return simple_cmd(argv);
}

bool search_equal(char* arg){
	int i = 0;
	bool found = false;
	char* copie = malloc (sizeof(char)*100);
	char* copie2 = malloc (sizeof(char)*100);
	char* word = strpbrk(arg, "=");
	if(word != NULL){
		while (arg[i]!='='){
				copie[i] = arg[i];
				i++;
		}	
		copie[i] = '\0';
		strcpy (copie2, word+1);
		setenv(copie,copie2,1);
		found = true;
	}
	return found; 
}

int parse_line(char* s){
	
	char** argv;
	char* c;
	if(*s == '#' || *s == '\n') return EXIT_SUCCESS;
	if ((c=strpbrk(s,"#"))!= NULL){
		*c='\0';
	}
	argv = malloc(sizeof (char*) * 100);
	for (int n=0; n<100; n++){
		argv[n] =malloc(sizeof (char*) * 8);
	}
	bool equal_found = false;
	char* word;
	argv[0]=s;
	int compteur = 1;
	while((word = strpbrk (s," \n")) != NULL ){
			//printf("espace détecté : %d\n",compteur);
			*word = '\0';
			if(search_equal(argv[compteur-1])) equal_found = true;
			s = word + 1;
			argv[compteur] = (char *) s;
			//printf("argv[%d] = %s\n",compteur,s);
			//printf("argv : %s\n", argv[compteur]);
			compteur++;
	}
	int i = 0;
	argv[compteur-1] = NULL;
	
	if(equal_found) return EXIT_SUCCESS;

	i=0;
	while(argv[i] != NULL){
		if((strpbrk(argv[i],"$"))!=NULL){		
			argv[i] = getenv(++argv[i]);
		}
		i++;
	}

	char* argvcopie[100];
	i=1;
	int j = 0;
	while(argv[i] != NULL){
		if((strpbrk(argv[i], "<")) != NULL){
			for(j = 0; j < i; j++){
				argvcopie[j] = argv[i];
			}
			argvcopie[j] = NULL;
			redir_cmd(argvcopie, argv[i-1], SORTIE_STANDART);
		}
		if((strpbrk(argv[i], ">")) != NULL){
			if(argv[i+1] != NULL){
				j = i+1;
				while(argv[j] != NULL){
					argvcopie[j - i] = argv[j];
					j++;
				}
				redir_cmd(argvcopie, argv[i+1], SORTIE_STANDART);
			} else {
				fprintf(stderr, "Argument attendu après '>'\n");
			}
		}
	}

 	int b  = simple_cmd(argv);
	return b;

}

int main (int argc, char** argv){
	int i ;
	char* buf = malloc(BUFSIZE * sizeof(char));
	int fd;
	char t[MAXARGS];
	for(i = 1; i < argc; i++){
		fd = open(argv[i], O_RDONLY);
		if(fd < 0){
			perror(argv[i]);	
		} 
		read(fd,buf,BUFSIZE);
		char* word = NULL;
		char* copie = malloc(MAXARGS * sizeof(char));
	//	int compteur = 1;
		while((word = strpbrk (buf,"\n")) != NULL){ 
			*word = '\0';
			strcpy(copie, buf);
			if(copie[strlen(copie) - 1] != '\n') strcat(copie, "\n");
			parse_line(copie);
			buf = word + 1;
	//		compteur++;
		}
	}
	do {	
		printf("$");
		fgets(t, sizeof(t), stdin);
		 parse_line(t);
	} while(1);
	
	free(buf);
	close(fd);

	return 0;

}

