/* 118924120 */

#include <stdio.h>
#include <sys/types.h>
#include <sysexits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "command.h"
#include "executor.h"

#define OPEN_FLAGS (O_WRONLY | O_TRUNC | O_CREAT)
#define DEF_MODE 0664

int execute_aux(struct tree *t, int p_input_fd, int p_output_fd);
static void print_tree(struct tree *t);
static void error_check(int status, const char *error_message);

int execute(struct tree *t) {
    /* print_tree(t); */ 
    /* root node */
    return execute_aux(t, STDIN_FILENO, STDOUT_FILENO);
}

/* use this to process the tree recursively */
int execute_aux(struct tree *t, int p_input_fd, int p_output_fd) {
    int pipe_fd[2], status, fd;
    pid_t res1, res2, result;

   if(t->conjunction == NONE) {
        /* shell commands */
        if(!strcmp(t->argv[0], "exit")) {
            exit(0);
        } else if(!strcmp(t->argv[0], "cd")) { 
            /* if no args, change to home directory */
            if(t->argv[1] == NULL) {
                const char *home = getenv("HOME");
                if(chdir(home) == -1) {
                perror(home);
                }
            } else {
                if(chdir(t->argv[1]) == -1) {
                perror(t->argv[1]);
                }
            }
        /* linux commands */
        } else {
            status = (result = fork());
            error_check(status, "fork error");
            /* parent */
            if(result != 0) {
                int status;
                wait(&status);
                /* child process command succesfully  */
                if(WIFEXITED(status)) {
                    return WEXITSTATUS(status);
                } 
	 	return 1;
            } else { /* child */
                /* i/o redirection */
                if(t->input != NULL) {
                    status = fd = open(t->input, O_RDONLY);
                    error_check(status, "opening error");

                    status = dup2(fd, STDIN_FILENO);   
                    error_check(status, "dup2 error");

                    status = close(fd);
                    error_check(status, "closing error");
                } else if(p_input_fd != STDIN_FILENO) {
                    status = dup2(p_input_fd, STDIN_FILENO);   
                    error_check(status, "dup2 error");

                    status = close(p_input_fd);
                    error_check(status, "closing error");
                }
                if(t->output != NULL) {
                    status = fd = open(t->output, OPEN_FLAGS, DEF_MODE);
                    error_check(status, "opening error");

                    status = dup2(fd, STDOUT_FILENO);
                    error_check(status, "dup2 error");

                    status = close(fd);
                    error_check(status, "closing error");
                } else if(p_output_fd != STDOUT_FILENO) {
                    status = dup2(p_output_fd, STDOUT_FILENO);   
                    error_check(status, "dup2 error");

                    status = close(p_output_fd);
                    error_check(status, "closing error");
                }
                /* exec */
                execvp(t->argv[0], t->argv);
	        fprintf(stderr, "Failed to execute %s\n", t->argv[0]); 
                fflush(stdout);
                exit(EX_OSERR);
	    }
        }
    } else if(t->conjunction == AND) {
        int left_status = execute_aux(t->left, p_input_fd, p_output_fd);
        if (left_status == 0) {
            int right_status = execute_aux(t->right, p_input_fd, p_output_fd);
            return right_status;
        }
        return left_status;
    } else if(t->conjunction == PIPE) {
	int value;
	/* check for ambiguous redirect */
	if(t->left->output != NULL) {
	    printf("Ambiguous output redirect.\n");
	    fflush(stdout);
	    return 1; 
	}
        if(t->right->input != NULL) {
            printf("Ambiguous input redirect.\n");
            fflush(stdout);
            return 1;
        }
	
        status = pipe(pipe_fd);
        error_check(status, "pipe error");

        status = res1 = fork();
        error_check(status, "fork error");

        /*child */
        if(res1 == 0) {
            /* left node doesn't need read end */
            status = close(pipe_fd[0]);
	    error_check(status, "close error");

            value = execute_aux(t->left, p_input_fd, pipe_fd[1]);

            status = close(pipe_fd[1]);
	    error_check(status, "close error");

	    exit(value);
        } else { 
            status = close(pipe_fd[1]);
            error_check(status, "close error");

            execute_aux(t->right, pipe_fd[0], p_output_fd);
                
	    status = close(pipe_fd[0]);
	    error_check(status, "close error");
               
            waitpid(res1, NULL, 0);
            
        }
    } else if(t->conjunction == SUBSHELL) {
        status = result = fork();
        error_check(status, "fork error");

        if(result == 0) {
            /* i/o redirection */
            if(t->input != NULL) {
              status = fd = open(t->input, O_RDONLY);
              error_check(status, "opening error");

              status = dup2(fd, STDIN_FILENO);   
              error_check(status, "dup2 error");

              status = close(fd);
              error_check(status, "closing error");
            } else if(p_input_fd != STDIN_FILENO) {
              status = dup2(p_input_fd, STDIN_FILENO);
              error_check(status, "dup2 error");
                    
              status = close(p_input_fd);
              error_check(status, "closing error");
            }
            if(t->output != NULL) {
              status = fd = open(t->output, OPEN_FLAGS, DEF_MODE);
              error_check(status, "opening error");

              status = dup2(fd, STDOUT_FILENO);
              error_check(status, "dup2 error");

              status = close(fd);
              error_check(status, "closing error");
            } else if(p_output_fd != STDOUT_FILENO) {
              status = dup2(p_output_fd, STDOUT_FILENO);
              error_check(status, "dup2 error");

              status = close(p_output_fd);
              error_check(status, "closing error");
            }
            status = execute_aux(t->left, STDIN_FILENO, STDOUT_FILENO);
            exit(status);
        } else {
            waitpid(result, NULL, 0);
        }
    }
    return 0; 
}

static void error_check (int status, const char *error_message) {
   if(status < 0) {
      perror(error_message); 
      exit(EX_OSERR);
   }
}
static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}
