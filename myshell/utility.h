/*
 * utility.h
 *
 *  Created on: Mar 12, 2018
 *      Author: redryerye
 */

#include <stdio.h>
/*
 * Using chdir(), exec(), pid_t, fork() and such
 */
#include <unistd.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>



#ifndef UTILITY_H_
#define UTILITY_H_

/*
 * Declraration of options
 */
int my_cd(char **args);
int my_clr(char **args);
int my_ls(char **args);
int my_dir(char **args);
int my_environ(char **args);
int my_echo(char **args);
int my_help(char **args);
int my_pwd(char **args);
int my_pause(char **args);
int my_file(char **args);
int my_quit(char **args);

/*
 * Option for the comman that is builtin to the system.
 */
char *bi_op[] = {
		"cd",
		"clr",
		"ls",
		"dir",
		"environ",
		"echo",
		"help",
		"pwd",
		"pause",
		"myshell",
		"quit"
};


/*
 * An array of function pointer
 *
 * Takes (an array of) String and returns int
 *
 */
int (*bi_fp[]) (char **) = {
  &my_cd,
  &my_clr,
  &my_ls,
  &my_dir,
  &my_environ,
  &my_echo,
  &my_help,
  &my_pwd,
  &my_pause,
  &my_file,
  &my_quit
};

int num_bi()
{
  return sizeof(bi_op) / sizeof(char *);
}

/*
 *  Function for 'cd'
 */
int my_cd(char **args)
{
  if(args[1] == NULL) {
    fprintf(stderr, "myshell: expected argument to \"cd\"\n");
  } else {
    if(chdir(args[1]) != 0) {
      perror("myshell");
    }
  }
  return 1;
}

/*
 * Function for clear command
 */
int my_clr(char **args) {
	system("cls");
	return 1;
}

/*
 * Function for ls command
 */
int my_ls(char **args)
{
	DIR *dir_path;
	struct dirent *direcotry;
	char *p = ".";

	// open dirent
	if((dir_path = opendir(p)) == NULL ){
		perror("Cannot open direcotry");
	}

	// load directory
	while((direcotry = readdir(dir_path)) != NULL){
		if(direcotry->d_ino == 0){
			/*
			 * ignore remove file
			 */
			continue;
		}
	    (void)printf("%s\n", direcotry->d_name);
	}
	// close dirent
	(void)closedir(dir_path);

	return 1;
}


/*
 * Function for dir (like ls command)
 */
void files(char *path);
void combine_path(char *p, const char *p1, const char *p2);

int my_dir(char **args)
{
	char path[256];

	strcpy(path, args[1]);
	files(path);

	return 1;
}

void files(char *path)
{
	DIR *dir;
	struct dirent *dir_path;
	struct stat f;
	char apath[256];

	dir = opendir(path);
	for (dir_path = readdir(dir); dir_path != NULL; dir_path = readdir(dir)) {
		if (dir_path->d_name[0] != '.') {
			combine_path(apath, path, dir_path->d_name);
			stat(apath, &f);
			if (!S_ISDIR(f.st_mode)) {
				printf("%s\n", apath);
			}
		}
	}
	closedir(dir);

	return;
}

void combine_path(char *p, const char *p1, const char *p2)
{
    strcpy(p, p1);
    strcat(p, "/");
    strcat(p, p2);

    return;
}



/*
 * Function to return all environment string
 */
int my_environ(char **args) {
	extern char** environ;

	for(int i=0 ; environ[i] != NULL ; i++ ) {
		printf("%s\n", environ[i] );
	}
	return 1;
}

/*
 * Function for echo
 */
int my_echo(char **args) {
	for(int i=1;i<20;i++) {
	    printf("%s ",args[i]);
	    if(args[i+1]==NULL) {
	    		break;
	    }
	}
	printf("\n");
	return 1;
}

/*
 * Function for 'help'
 */
int my_help(char **args)
{
  int i;
  printf("--Help--\n");
  printf("Built in commands:\n");

  for(i=0; i<num_bi(); i++) {
    printf("  %s\n", bi_op[i]);
  }

  printf("If you need more information, use the 'man' command or read README.\n");
  return 1;
}

/*
 * Function for pwd command
 */
int my_pwd(char **args) {
  char p[MAXPATHLEN];
	if(!getcwd(p, sizeof(p))) {
		// failure
		return -1;
	}
	printf("%s\n", p);

	return 1;
}

/*
 * Function to pause the program until the user press 'enter'
 */
int my_pause(char **args) {
	getchar();
	return 1;
}

/*
 * Function to read command from file
 */
int my_file(char **args) {
	FILE *fp;
	int chr;

	fp = fopen(args[1], "r");
	if(fp == NULL) {
		printf("%s file is not open\n", args[1]);
		return -1;
	}

	while((chr = fgetc(fp)) != EOF) {
		putchar(chr);
	}
	printf("\n");

	fclose(fp);

	return 1;
}

/*
 * Function for 'exit'
 */
int my_quit(char **args) {
	return 0;
}


#endif /* UTILITY_H_ */
