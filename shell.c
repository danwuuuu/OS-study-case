// Shell starter file
// You may make any changes to any part of this file.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<errno.h>
#include <signal.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)

#define BUFFER_SIZE 50
static char buffer[BUFFER_SIZE];
int flage=0;
/**
 * Command Input and Processing
 */

/*
 * Tokenize the string in 'buff' into 'tokens'.
 * buff: Character array containing string to tokenize.
 *       Will be modified: all whitespace replaced with '\0'
 * tokens: array of pointers of size at least COMMAND_LENGTH/2 + 1.
 *       Will be modified so tokens[i] points to the i'th token
 *       in the string buff. All returned tokens will be non-empty.
 *       NOTE: pointers in tokens[] will all point into buff!
 *       Ends with a null pointer.
 * returns: number of tokens.
 */
int tokenize_command(char *buff, char *tokens[])
{
	int token_count = 0;
	_Bool in_token = false;
	int num_chars = strnlen(buff, COMMAND_LENGTH);
	for (int i = 0; i < num_chars; i++) {
		switch (buff[i]) {
		// Handle token delimiters (ends):
		case ' ':
		case '\t':
		case '\n':
			buff[i] = '\0';
			in_token = false;
			break;

		// Handle other characters (may be start)
		default:
			if (!in_token) {
				tokens[token_count] = &buff[i];
				token_count++;
				in_token = true;
			}
		}
	}
	tokens[token_count] = NULL;
	return token_count;
}

/**
 * Read a command from the keyboard into the buffer 'buff' and tokenize it
 * such that 'tokens[i]' points into 'buff' to the i'th token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 *       COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into 'buff'. Must be at
 *       least NUM_TOKENS long. Will strip out up to one final '&' token.
 *       tokens will be NULL terminated (a NULL pointer indicates end of tokens).
 * in_background: pointer to a boolean variable. Set to true if user entered
 *       an & as their last token; otherwise set to false.
 */
void read_command(char *buff, char *tokens[], _Bool *in_background)
{
	*in_background = false;

	// Read input
	int length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1);

	if (length < 0 &&errno!=EINTR) {
		perror("Unable to read command from keyboard. Terminating.\n");
		exit(-1);
	}

	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}

	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
		return;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
}

void handle_SIGINT()
{
flage=2;
strcpy(buffer,"i got it");
}
/**
 * Main and Execute Commands
 */
int main(int argc, char* argv[])
{
	char input_buffer[COMMAND_LENGTH];
	char *tokens[NUM_TOKENS];
	char **history=(char**)malloc(500*sizeof(char*));
	int count=0;
	int begin=0;
        int repeat=0;
	
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	handler.sa_flags = 0;
	sigemptyset(&handler.sa_mask);
	sigaction(SIGINT, &handler, NULL);
	//strcpy(buffer,"i got it");

	while (true) {
		
		
		// Get command
		// Use write because we need to use read() to work with
		// signals, and read() is incompatible with printf().
		
		
		

		char cwd[200];
		getcwd(cwd,sizeof(cwd));
		_Bool in_background = false;

		flage=1;
		if(strcmp(buffer,"i got it")==0){
			count=count-1;
		        strcpy(tokens[0],"history");
			strcpy(buffer,"i tho it");
			flage=0;
			//write(STDOUT_FILENO, input_buffer, strlen(input_buffer));
			//write(STDOUT_FILENO, "\n", strlen("\n"));
			
			
		}else{

		if(repeat!=0){
		
		write(STDOUT_FILENO, input_buffer, strlen(input_buffer));
		write(STDOUT_FILENO, "\n", strlen("\n"));
		repeat=0;
		
		}
		else if(flage==0){ flage=1;}
		else{
			
		write(STDOUT_FILENO, cwd, strlen(cwd));
		write(STDOUT_FILENO, "> ", strlen("> "));
		read_command(input_buffer, tokens, &in_background);}}
		
		
		//exit command
		if(tokens[0]!=NULL)//DO NOTHING
		{
		///////////////////////////////////////////////////////////////
		
 		int len=strlen(input_buffer);
		history[count]=(char*)malloc(len*sizeof(char));
		
		strcpy(history[count],input_buffer);
		
		//write(STDOUT_FILENO, history[count], len);
		count++;
		///////////////////////////////////////////////////////////////
 		if(strcmp(tokens[0],"exit")==0)
		{
			//exe=false;
			
			  for (int i=0;i<count;i++){
	free(history[i]);}
	free(history);
exit (0);
			//return 0;
		}
		
		//pwd command 
             
		 else if(strcmp(tokens[0],"pwd")==0)		
 		{
			//exe=false;
   			char cwd[200];
			getcwd(cwd,sizeof(cwd));
         	}
		//cd command 
		
		else if(strcmp(tokens[0],"cd")==0)
		{
   			char cwd[200];
			getcwd(cwd,sizeof(cwd));
			if(tokens[1]!=NULL){

			strcat(cwd,tokens[1]);


		       if(chdir(cwd)!=0)
                          {     int len=strlen("Invalid directory \n");
				write(2,"Invalid directory \n",len);}
			else
				chdir(cwd);
			}

                }
		//////////////////////////////////////////////////////////////////////
 		else if(strcmp(tokens[0],"history")==0)		
 		{
    			
 			char strtoint[100];
			int index=begin;
			if(count<=10){
			begin=0;
			index=begin+1;
	 		}
			else
			{
				begin=count-11;
				index=begin+1;		
			}
			while(begin<count-1)
			
			{
			    sprintf(strtoint,"%d",index);
			    write(STDOUT_FILENO, strtoint, strlen(strtoint));
			    write(STDOUT_FILENO, "	", strlen("	"));
			    write(STDOUT_FILENO, history[begin], strlen(history[begin]));

				if(in_background==true)
				{
					write(STDOUT_FILENO, " ", strlen(" "));
					write(STDOUT_FILENO, "&", strlen("&"));
				}
			    write(STDOUT_FILENO, "\n", strlen("\n"));
			    begin++;
			    index++;
			}
         	}
		else if(tokens[0][0]=='!')		
 		{
			int len=strlen(tokens[0]);
			len=len-1;
			if(count>1&&len>0){
			char sub[len+1];
			memcpy(sub,tokens[0]+1,len);
			sub[len+1]='\0';//sub string
			int i=atoi(sub);
			if(tokens[0][1]=='!'){
				strcpy(input_buffer,history[count-2]);
				count=count-1;
			        free(history[count]);
				repeat=1;
				//write(STDOUT_FILENO, input_buffer, strlen(input_buffer));

				
			}
     			else if (i==0)
			{
				write(2,"SHELL: Unknown history command\n",strlen("SHELL: Unknown history command\n")); 
				
		 		count=count-1;
				free(history[count]);
			
			}
			else if(count>i){//ligal case 
				strcpy(input_buffer,history[i-1]);
				
				//write(STDOUT_FILENO, history[i-1], strlen(history[i-1]));
				repeat=2;
				count=count-1;
				free(history[count]);
				
			continue;
			}
			else 
			{
				write(2,"SHELL: Unknown history command\n",strlen("SHELL: Unknown history command\n")); 
				count=count-1;
				free(history[count]);
			}

			}


			else//illegal case for nothing follow !
			{
				write(2,"SHELL: Unknown history command\n",strlen("SHELL: Unknown history command\n")); 
					count=count-1;
				free(history[count]);
			}
			
		
		}
		else
	 	{
			pid_t pid;
		
		pid=fork();
		if(pid<0)
		{
			fprintf(stderr, "fork failed" );
		}


		else if (pid==0)//child process
		{
			
			int result=execvp(tokens[0],tokens);

			if(result==-1)//error
			{
				int len=strlen(":Unkonw command.\n");
				write(2,input_buffer,strlen(input_buffer));
				write(2,":Unkonw command.\n",len);   				
			}
		}
		else
		{
			if(in_background==false)
				waitpid(pid,NULL,0);
		}
		
		while (waitpid(-1, NULL, WNOHANG) >0) ;
		}
	}
}

       for (int i=0;i<count;i++){
	free(history[i]);}
	free(history);

	return 0;
}
