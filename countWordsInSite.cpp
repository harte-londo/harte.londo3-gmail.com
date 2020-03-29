#include        <stdlib.h>
#include        <stdio.h>
#include        <string.h>
#include        <unistd.h>      
#include        <errno.h>
#include        <sys/socket.h>
#include        <sys/types.h>   
#include        <sys/wait.h>    
#include        <sys/stat.h>    
#include        <fcntl.h>       
#include        <signal.h>
#include        <ncurses.h>


#define DEFAULT_PATH "/usr/bin/hunspell"
#define DEFAULT_LANG "en_US"

pid_t child1;
pid_t child2;
int child1IO;
int child2IO;
const int BUFFER_LEN = 1024;
char inputBuffer[BUFFER_LEN];

int main(int argc, char* argv[]){
	int fd[2];
	if(socketpair(AF_UNIX,SOCK_STREAM,0,fd) < 0){
		fprintf(stderr,"Socket() failed: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	child1 = fork();
	if(child1 < 0){
		fprintf(stderr,"Fork() failed: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(child1 == 0){
		close(fd[1]);
		child1IO = fd[0];
		dup2(fd[0],STDOUT_FILENO);
		dup2(fd[0],STDIN_FILENO);
		if(argc ==3){
			execl(DEFAULT_PATH,DEFAULT_PATH,"-a","-d",DEFAULT_LANG,NULL);
			fprintf(stderr,"1 Cannot execl(): %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		if(argc == 4){
			execl(argv[3],argv[3],"-a","-d",argv[4],NULL);
			fprintf(stderr,"1.1 Cannot execl(): %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	if(child1 > 0){
		child2 = fork();
		if(child2 == 0){
			close(fd[0]);
			child2IO = fd[1];
			dup2(fd[1],STDOUT_FILENO);
			dup2(fd[1],STDIN_FILENO);
			execl("/usr/local/bin/java","java","javaPart",argv[1],argv[2],NULL);
			fprintf(stderr,"2 Cannot execl(): %s\n",strerror(errno));
			exit(EXIT_FAILURE);
		}else{	
			int returnStatus;
			close(fd[0]);
			close(fd[1]);
			wait(NULL);
			kill(child1,SIGINT);
			wait(NULL);				
			FILE *fPtr;
			char c;
			fPtr = fopen("output.txt","r");
			if(fPtr == NULL){
				printf("Nothing found");
				return(EXIT_FAILURE);
			}
			c = fgetc(fPtr);
			while(c != EOF){
				printf("%c",c);
				c = fgetc(fPtr);	
			}
			fclose(fPtr);							
		}
	}
	return(EXIT_SUCCESS);
}

