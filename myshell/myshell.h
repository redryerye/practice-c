/*
 * myshell.h
 *
 *  Created on: Mar 12, 2018
 *      Author: redryerye
 */

#include "utility.h"

#include <sys/wait.h>
#include <string.h>


#define MY_BUFSIZE 1024
#define MY_TOKEN_BUFSIZE 64
#define MY_TOKEN " \t\r\n\a"

#ifndef MYSHELL_H_
#define MYSHELL_H_

/*
 * Takes function arguments and fork the process
 * Start the program
 */
int my_start(char **args)
{
  pid_t pid, apid;
  int status;

  pid = fork();

  if(pid == 0) {
    /*
     * Child here
     * run command given by user
     */
    if(execvp(args[0], args) == -1) {
      perror("myshell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // error
    perror("myshell");
  } else {
    do {
      apid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  // return 1 to continue
  return 1;
}

/*
 * launch internal command or process
 */
int my_e(char **args)
{
	/*
	 * Contain NULL / Space ?
	 */
  if(args[0] == NULL) {
    return 1;
    // continue run
  }

  for(int i=0; i<num_bi(); i++) {
    if(strcmp(args[0], bi_op[i]) == 0) {
      return (*bi_fp[i])(args);
    }
  }
  return my_start(args);
  /*
   * if it does not match the internal commands, call my_start
   */
}

/*
 * read line from stdin..
 * allocate block and reallocate if needed.
 */
char *my_rline(void)
{
  int bs = MY_BUFSIZE;
  int getc;
  int pos = 0;
  char *buf = malloc(sizeof(char) * bs);

  while(1) {
    getc = getchar();
    // EOF is an integer
    if(getc == EOF || getc == '\n') {
      buf[pos] = '\0';
      return buf;
    } else {
      buf[pos] = getc;
    }
    pos++;
    // exceed bufsize?
    if(pos >= bs) {
      bs += MY_BUFSIZE;
      buf = realloc(buf, bs);

      if(!buf) {
    	  	// unsuccessful reallocation
        fprintf(stderr, "myshell: allcoation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

/*
 * Parse the line
 * null
 */
char **my_sline(char *rline)
{
  int bs = MY_TOKEN_BUFSIZE, pos = 0, tlen;
  char **tokens = malloc(bs * sizeof(char));
  char *token, *token_copy;

  /*
   * tokenize using delimiter
   * split!
   */
  token = strtok(rline, MY_TOKEN);

  while(token != NULL) {
    tlen = strlen(token);
    token_copy = malloc((tlen + 1) * sizeof(char));
    strcpy(token_copy, token);

    tokens[pos] = token_copy;
    pos++;

    if(pos <= bs) {
      bs += MY_TOKEN_BUFSIZE;
      tokens = realloc(tokens, bs);
      if(!tokens) {
    	  	// unsuccessful reallocation
        fprintf(stderr, "myshell: allcoation error\n");
        exit(EXIT_FAILURE);
      }
    }
    // return pointer of first token
    token = strtok(NULL, MY_TOKEN);
  }
  tokens[pos] = NULL;
  return tokens;
}

/*
 * free args function
 */
void my_free(char **args)
{
  char **itr = args;
  while(*itr != NULL) {
    free(*itr);
    itr++;
  }
  free(args);
}

/*
 * Continuously loop through
 */
void my_con()
{
  char *line;
  char **args;
  int st;

  do
  {
    char p[MAXPATHLEN];
	getcwd(p, sizeof(p));

    	printf("%s/myshell> ", p);
    line = my_rline();
    args = my_sline(line);
    st = my_e(args);

    free(line);
    my_free(args);
  } while(st);

}

#endif /* MYSHELL_H_ */
