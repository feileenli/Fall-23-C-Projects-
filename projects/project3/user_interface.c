/* Feileen Li, 118924120, fli1234 */

#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include <ctype.h>
#include "document.h"
#define MAX_LEN 80

static void process_input(Document *doc, FILE *input, int argc);
static int command_add_paragraph(char *line, Document *doc);
static int command_add_line(char *line, Document *doc);
static int command_append(char *line, Document *doc);
static int command_remove(char *line, Document *doc);
static int command_load(char *line, Document *doc);
static int command_print(char *line, Document *doc);
static int command_quit(char *line);
static int command_exit(char *line);
static int command_replace(char *line, Document *doc);
static int command_highlight(char *line, Document *doc);
static int command_remove_txt(char *line, Document *doc);
static int command_save(char *line, Document *doc);
static int command_reset(char *line, Document *doc);
static int valid_line(char *line);

int main(int argc, char *argv[]) {
  FILE *input;
  Document doc;
 
  /* if arg is 1, input is stdin. if arg is 2, input is file */
  if(argc == 1) { 
    printf("> ");
    process_input(&doc, stdin, argc);  
  } else if(argc == 2) {
     input = fopen(argv[1], "r");
     if(input == NULL) {
       fprintf(stderr, "%s cannot be opened.\n", argv[1]); 
       exit(EX_OSERR); 
     } 
     process_input(&doc, input, argc);
     fclose(input);
  } else {
    fprintf(stderr, "Usage: %s\n", argv[0]);
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    exit(EX_USAGE);
  }
  return EXIT_SUCCESS;
}

/* process file or user input */
static void process_input(Document *doc, FILE *input, int argc) {
  char line[MAX_LEN + 1];
  const char name[] = "main_document";

  init_document(doc, name);
  while(fgets(line, MAX_LEN + 1, input)) {
    if(strlen(line) <= 1024) {
      /* if it's a comment or a blank line, continue */
       if(valid_line(line)) { 
          if(argc == 1) {
            printf("> ");
          }      
          continue; 
       }
      /* if it is not any of the commands */
      if(!command_add_paragraph(line, doc) && !command_add_line(line, doc) && 
         !command_print(line, doc) && !command_append(line, doc) && 
         !command_remove(line, doc) && !command_load(line, doc) &&
         !command_replace(line, doc) && !command_highlight(line, doc) &&
         !command_remove_txt(line, doc) && !command_save(line, doc) &&
         !command_reset(line, doc) && !command_quit(line) &&
         !command_exit(line) ) {
        printf("Invalid Command\n");
          if(argc == 1) {
            printf("> ");
          }
        continue;
      }
      if(command_quit(line)) {
        break;
      }
      if(command_exit(line)) {
        break;
      }
    } else {
      /* over char count */
      printf("Invalid Command\n");
    }
    if(argc == 1) {
      printf("> ");
    }
  }
  return; 
}

/* check if line is valid */
static int valid_line(char *line) {
  char *p = line;

  /* first non white space is # or is blank line */
  while(*p) {
    if(!isspace(*p)) {
      if(*p == '#') {
        return 1; 
      } else {
        return 0;
      }
    } 
    p++;
  }
  return 1; 
}

/* add a paragraph to the doc */
static int command_add_paragraph(char *line, Document *doc) {
  const char *p;
  int result, paragraph_num = -1;
  char extra[1024]; 

 /* check if the line contains this command and a paragraph number */
 if((p = strstr(line, "add_paragraph_after"))) {
   p += strlen("add_paragraph_after"); 
   if(p && sscanf(p, "%d", &paragraph_num) == 1) {
     if(paragraph_num >= 0 && sscanf(p, "%d %s", &paragraph_num, extra) == 1) {
        result = add_paragraph_after(doc, paragraph_num);
          if (result == FAILURE) {
            printf("add_paragraph_after failed\n");
          }
          return 1;
     }
   }   
 }
 return 0;
}

/* add a line after the line w the specified line number */
static int command_add_line(char *line, Document *doc) {
  const char *p;
  const char *l; 
  int result, paragraph_num = -1, line_num = -1; 
  char new_line[MAX_LEN + 1]; 

  /* check if the line contains this command and a paragraph number */
  if((p = strstr(line, "add_line_after"))) {
    p += strlen("add_line_after");
    if(p && sscanf(p, "%d %d", &paragraph_num, &line_num) == 2) {
      if(paragraph_num > 0 && line_num >= 0 && (l = strstr(line, "*"))) {
        l++;
        strcpy(new_line, l);
        new_line[strlen(new_line) - 1] = '\0';
        result = add_line_after(doc, paragraph_num, line_num, new_line);
        if(result == FAILURE) {
          printf("add_line_after failed\n");
        }
        return 1; 
      }
    } 
  }
  return 0;
}

/*print the document */
static int command_print(char *line, Document *doc) {
  char *p, extra[1024];
  int result;

  /*check if the line contains this command */
  if((p = strstr(line, "print_document"))) {
    p += strlen("print_document");
    if(sscanf(p, "%s", extra) != 1) {
      result = print_document(doc);
      if(result == FAILURE) {
        printf("print_document failed\n");
      }
      return 1;
    }
  }
  return 0;
}

/* exit the user interface */
static int command_quit(char *line) {
  char *p, extra[1024];

  /*check if the line contains this command */
  if((p = strstr(line, "quit"))) {
    p += strlen("quit");
    if(sscanf(p, "%s", extra) != 1) {
      return 1;
    }
  }
  return 0;

}

/* exit the user interface */
int command_exit(char *line) {
  char *p, extra[1024];

  /*check if the line contains this command */
  if((p = strstr(line, "exit"))) {
    p += strlen("exit");
    if(sscanf(p, "%s", extra) != 1) {
      return 1;
    }
  }
  return 0;

}

/* append a line to the specified paragraph */
static int command_append(char *line, Document *doc) {
  const char *p;
  const char *l; 
  int result, paragraph_num = 0; 
  char new_line[MAX_LEN + 1]; 

 /* check if the line contains this command and a paragraph number */
  if((p = strstr(line, "append_line"))) {
    p += strlen("append_line");
    if(p && sscanf(p, "%d", &paragraph_num) == 1) {
      if(paragraph_num > 0 && (l = strstr(line, "*"))) {
        l++;
        strcpy(new_line, l);
        new_line[strlen(new_line) - 1] = '\0';
        result = append_line(doc, paragraph_num, new_line);
        if(result == FAILURE) {
          printf("append_line failed\n");
        }
        return 1;
      }
    }
  }
  return 0;
}

/* remove the specified line from the paragraph */
static int command_remove(char *line, Document *doc) {
  const char *p;
  char extra[1024];
  int result, paragraph_num = 0, line_num = 0; 

 /* check if the line contains this command and a paragraph number */
  if((p = strstr(line, "remove_line"))) {
    p += strlen("remove_line");
    if(sscanf(p, "%d %d", &paragraph_num, &line_num) == 2) {
      if(paragraph_num > 0 && line_num > 0 && sscanf(p, "%d %d %s", &paragraph_num, &line_num, extra) == 2) {
        result = remove_line(doc, paragraph_num, line_num);
        if(result == FAILURE) {
          printf("remove_line failed\n");
        }
        return 1;
      }
    }
  }
  return 0;
}

/* load the specified file into the current document */
static int command_load(char *line, Document *doc) {
  char *p;
  char file[MAX_LEN + 1], extra[1024];   
  int result; 

 /* check if the line contains this command */
 if((p = strstr(line, "load_file"))) {
   p += strlen("load_file");
   if(sscanf(p, "%s %s", file, extra) == 1) {
     result = load_file(doc, file);
       if(result == FAILURE) {
         printf("load_file failed\n");
       }
       return 1;
   }
 }
 return 0;
}

/* replace the target string with replacement */
static int command_replace(char *line, Document *doc) {
  char *p, *q1, *q2;
  char target[MAX_LEN + 1], replace[MAX_LEN + 1];
  int result;

  /*check if the line contains this command */
  if((p = strstr(line, "replace_text"))) {
    p += strlen("replace_text");
    q1 = strstr(p, "\"");
    if(q1) {
      q2 = strstr(q1 + 1, "\"");
      if(q2) {
        strncpy(target, q1 + 1, q2 - q1 - 1);
        target[q2-q1-1] = '\0';
        q1 = strstr(q2 + 1, "\"");
        if(q1) {
          q2 = strstr(q1 + 1, "\"");
          if(q2) {
            strncpy(replace, q1 + 1, q2 - q1 - 1);
            replace[q2-q1-1] = '\0';
            result = replace_text(doc, target, replace);
            if(result == FAILURE) {
              printf("replace_text failed");
            }
            return 1;
          }
        }
      } 
    }
  }
 return 0;
}


/* highlight the target string */
static int command_highlight(char *line, Document *doc) {

  char *p, *q1, *q2;
  char target[MAX_LEN + 1];
  int result;

  /*check if the line contains this command */
  if((p = strstr(line, "highlight_text"))) {
    p += strlen("highlight_text");
    q1 = strstr(p, "\"");
    if(q1) {
      q2 = strstr(q1 + 1, "\"");
      if(q2) {
        strncpy(target, q1 + 1, q2 - q1 - 1);
        target[q2-q1-1] = '\0';
        result = highlight_text(doc, target);
        if(result == FAILURE) {
          printf("highlight_text failed");
        }
        return 1;
      }
    }
  }
   return 0;
}

/* remove the target string */
static int command_remove_txt(char *line, Document *doc) {
 
  char *p, *q1, *q2;
  char target[MAX_LEN + 1];
  int result;

  /*check if the line contains this command */
  if((p = strstr(line, "remove_text"))) {
    p += strlen("remove_text");
    q1 = strstr(p, "\"");
    if(q1) {
      q2 = strstr(q1 + 1, "\"");
      if(q2) {
        strncpy(target, q1 + 1, q2 - q1 - 1);
        target[q2-q1-1] = '\0';
        result = remove_text(doc, target);
        if(result == FAILURE) {
          printf("remove_text failed");
        }
        return 1;
      }
    }
  }
   return 0;
}

/* save the current document to the specified file */
static int command_save(char *line, Document *doc) {
  
  char *p;
  char file[MAX_LEN + 1], extra[1024];
  int result;

  /* check if the line contains this command */
  if((p = strstr(line, "save_document"))) {
    p += strlen("save_document");
    if(sscanf(p, "%s %s", file, extra) == 1) {
      result = save_document(doc, file);
        if(result == FAILURE) {
          printf("save_document failed\n");
        }
        return 1;
    }
  }
  return 0;
}

/* reset the current document */
static int command_reset(char *line, Document *doc) {
  char *p, extra[1024];
  int result;
  
  /*check if the line contains this command */
  if((p = strstr(line, "reset_document"))) {
    p += strlen("reset_document");
    if(sscanf(p, "%s", extra) != 1) {
      result = reset_document(doc);
      if(result == FAILURE) {
          printf("reset_document failed\n");
        }
        return 1;
    }
  }
  return 0;
}

