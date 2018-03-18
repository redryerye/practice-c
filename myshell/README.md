The Project is based on a tutorial by Stephen Brennan(https://brennan.io/2015/01/16/write-a-shell-in-c/).

makefile not included.

Please gcc it yourself.

Introduction to Systems Programming & Operating Systems
Project Two: Developing a Shell Program
by Yuki Yamamoto

This is a original shell program that contains some of the UNIX commands.

List of commands:

1. cd <directory> - Change the current default directory to <directory>.
					If the <directory> argument is not present, report the current directory.
					If the directory does not exist an appropriate error should be reported.
					This command should also change the PWD environment variable.

2. clr - Clear the screen. **ONLY works in Linux since it is using system("cls")

3. ls - list all the file in the current directory

4. dir <directory> - List the contents of directory <directory>.

5. environ - List all the environment strings.

6. echo <comment> - Display <comment> on the display followed by a new line.

7. help - Display the user manual and instruction

8. pwd - Show current route directory

9. pause - Pause operation of the shell until 'Enter' is pressed.

10. Read file and print it out.

11. quit - Quit the shell.
