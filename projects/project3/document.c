#include <stdio.h> 
#include "document.h"
#include <string.h>
#include <ctype.h>

/* function prototypes */
char *replace_str(const char *line, const char *target, const char *replacement,
char *updated_line);
char *remove_aux(const char *line, const char *target,
char *updated_line);
int isblank(const char *line);

/* Initializes the document to be empty. An empty document is one with 0 
 * paragraphs. */
int init_document(Document *doc, const char *name) {
/* The function will return FAILURE if doc is NULL, name is NULL, or if the 
 * length of the name provided exceeds MAX_STR_SIZE */
  if(doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
    return FAILURE;
  } 
  strcpy(doc->name, name);
  doc->number_of_paragraphs = 0;
  return SUCCESS;
}

/* Sets the number of paragraphs to 0. */
int reset_document(Document *doc) {
  if(doc == NULL) {
    return FAILURE;
  } 
  doc->number_of_paragraphs = 0;
  return SUCCESS;
}

/* Prints the document’s name, number of paragraphs, followed by the paragraphs. 
 * Each paragraph is separated by a blank line. */
int print_document(Document *doc) {
 
  int i, j;
  if(doc == NULL) {
    return FAILURE;
  }
  printf("Document name: \"%s\"\n", doc->name);
  printf("Number of Paragraphs: %d\n",  doc->number_of_paragraphs);
  for(i = 0; i < doc->number_of_paragraphs; i++) {
    if(i != 0) {
      printf("\n");
    }
    for(j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      printf("%s\n", doc->paragraphs[i].lines[j]);
    }
  }
  return SUCCESS;
}

/* Adds a paragraph after the specified paragraph number. Paragraph numbers 
 * start at 1. If paragraph number is 0 the paragraph will be added at the 
 * beginning of the document. */
int add_paragraph_after(Document *doc, int paragraph_number) {
  
  int i;
/* The function will return FAILURE
 * if doc is NULL, the document has the maximum number of paragraphs allowed 
 * or if the paragraph number is larger than the number of paragraphs 
 * available; */
  if(doc == NULL || doc->number_of_paragraphs >= MAX_PARAGRAPHS || 
    paragraph_number > doc->number_of_paragraphs + 1) {
    return FAILURE;
  }
/* loop through from last paragraph to target paragraph and shift 
 * everything down one */
  for(i = doc->number_of_paragraphs; i > paragraph_number; i--) {
    doc->paragraphs[i] = doc->paragraphs[i-1]; 
  }
  doc->number_of_paragraphs++;
  doc->paragraphs[paragraph_number].number_of_lines = 0;
  return SUCCESS; 
}

/* Adds a new line after the line with the specified line number. Line numbers
 * start at 1. If the line number is 0, the line will be added at the beginning
 * of the paragraph.*/
int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line) {
  
  int i;
  if(doc == NULL || paragraph_number > doc->number_of_paragraphs || 
  doc->paragraphs[paragraph_number - 1].number_of_lines >= MAX_PARAGRAPH_LINES ||
  line_number > doc->paragraphs[paragraph_number - 1].number_of_lines ||
  new_line == NULL) {
    return FAILURE;
  }
/* same concept as add paragraph after */ 
  for(i = doc->paragraphs[paragraph_number - 1].number_of_lines; 
  i > line_number; i--) {
   strcpy(doc->paragraphs[paragraph_number - 1].lines[i], 
   doc->paragraphs[paragraph_number - 1].lines[i - 1]);
  }
  doc->paragraphs[paragraph_number - 1].number_of_lines++;
  strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line);
  return SUCCESS; 
}

/* Returns the number of lines in a paragraph using the number of lines out 
 * parameter. */
int get_number_lines_paragraph(Document *doc, int paragraph_number, 
                               int *number_of_lines) {
  if(doc == NULL || number_of_lines == NULL || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
  return SUCCESS;
}

/* Appends a line to the specified paragraph. */
int append_line(Document *doc, int paragraph_number, const char *new_line) {
  if(doc == NULL || paragraph_number > doc->number_of_paragraphs ||
  doc->paragraphs[paragraph_number - 1].number_of_lines >= MAX_PARAGRAPH_LINES ||
  new_line == NULL) {
    return FAILURE;
  }
/* copy the new line into the paragraph */
  strcpy(doc->paragraphs[paragraph_number - 1].
  lines[doc->paragraphs[paragraph_number - 1].number_of_lines], new_line);  
  doc->paragraphs[paragraph_number - 1].number_of_lines++;
  return SUCCESS;
}

/* Removes the specified line from the paragraph. */
int remove_line(Document *doc, int paragraph_number, int line_number) {

  int i;
  if(doc == NULL || paragraph_number > doc->number_of_paragraphs || 
  line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
    return FAILURE;
  }
/* test for if line is the last one.
 * shift everything left one */
  for(i = line_number - 1;
  i < doc->paragraphs[paragraph_number - 1].number_of_lines - 1; i++) {
    strcpy(doc->paragraphs[paragraph_number - 1].lines[i], 
    doc->paragraphs[paragraph_number - 1].lines[i+1]);
  }
  doc->paragraphs[paragraph_number - 1].number_of_lines--;
  return SUCCESS; 
}

/* The function will add the first data lines number of lines from the data 
 * array to the document. An empty string in the array will create a new 
 * paragraph.*/
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], 
                  int data_lines) {

  int i, num_paragraphs = 1, line_index = 0;
  if(doc == NULL || data == NULL || data_lines == 0) {
    return FAILURE;
  }
  add_paragraph_after(doc, 0);
  /* loop through data array document */
  for(i = 0; i < data_lines; i++) {
  /* if it's blank, start new paragraph */
   if(strlen(data[i]) == 0) {
     if(line_index > 0) {
     add_paragraph_after(doc, num_paragraphs);
     num_paragraphs++;
     line_index = 0;
     }
  /* else copy over the line */
  } else {  
    strcpy(doc->paragraphs[num_paragraphs - 1].lines[line_index], 
    data[i]);
    doc->paragraphs[num_paragraphs - 1].number_of_lines++;
    line_index++;
  }
  } 
  return SUCCESS;
}

/* The function will replace the text target with the text replacement 
 * everywhere it appears in the document. */
int replace_text(Document *doc, const char *target, const char *replacement) {

  int i, j;
  char updated_line[MAX_STR_SIZE + 1];
  if(doc == NULL || target == NULL || replacement == NULL) {
    return FAILURE;
  }
  for(i = 0; i < doc->number_of_paragraphs; i++) {
    for(j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      strcpy(doc->paragraphs[i].lines[j], 
      replace_str(doc->paragraphs[i].lines[j], target, replacement, updated_line)); 
    }
  }
  return SUCCESS;
}

char *replace_str(const char *line, const char *target, const char *replacement, 
                  char *updated_line) {
  const char *start_substr = line;
  char *result = updated_line; 
  const char *q = line;
  int i, leftover, count = 0;

  /* figure out how many occurences of target in the line */  
  do {
    q = strstr(q, target); 
    if (q) {
       q+= strlen(target); 
       count++; 
    }
  } while(q);

 /* for each occurence of the target, locate the end of the substring that 
 * doesnt contain the target. then copy that substring into the result.
 * then move the pointer to the end of the substring, and copy the 
 * replacement, move the pointer to the end of the substring again. 
 * update the start_substr so it points to the end of the substring */
 
  for(i = 0; i < count; i++) {
    const char *end_substr = strstr(start_substr, target); 
    int substr_length = end_substr - start_substr;
    memcpy(result, start_substr, substr_length);
    result += substr_length; 
    memcpy(result, replacement, strlen(replacement)); 
    result += strlen(replacement);
    start_substr = end_substr + strlen(target); 
  } 
  /* now that you've replaced every target, add the remnants of the line*/ 

  leftover = strlen(line) - (start_substr - line) + 1; 
  memcpy(result, start_substr, leftover); 
  return updated_line; 
}

/* The function will highlight the text associated with target everywhere it 
 * appears in the document by surrounding the text with the strings HIGHLIGHT 
 * START STR and HIGHLIGHT END STR */
int highlight_text(Document *doc, const char *target) { 

  char replacement[MAX_STR_SIZE + 1];
  const char left_bracket[MAX_STR_SIZE + 1] = HIGHLIGHT_START_STR;
  const char right_bracket[MAX_STR_SIZE + 1] = HIGHLIGHT_END_STR; 
  
  if(doc == NULL || target == NULL) {
    return FAILURE;
  }
  /* create a replacemet variable with the brackets and call replace text
   * function */
  strcpy(replacement,left_bracket);
  strcat(replacement, target);
  strcat(replacement, right_bracket);
  replace_text(doc, target, replacement); 
  return SUCCESS;
}

/* The function will remove the text target everywhere it appears in the 
 * document.*/
int remove_text(Document *doc, const char *target) {

 int i, j;
 char updated_line[MAX_STR_SIZE + 1];
 if(doc == NULL || target == NULL) {
   return FAILURE;
 } 
 for(i = 0; i < doc->number_of_paragraphs; i++) {
    for(j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      strcpy(doc->paragraphs[i].lines[j], 
      remove_aux(doc->paragraphs[i].lines[j], target, updated_line));
    }
  }
  return SUCCESS;
}

/* similar logic to replace text function */
char *remove_aux(const char *line, const char *target, 
                 char *updated_line) {
 
  /* create pointer variables that point at the first character in the line 
   * and the last character before the target character 
   * create the substring of stuff to be copied before the target*/ 
  char *result = updated_line; 
  const char *q = line;
  const char *start_substr = line;
  int i, leftover, count = 0;
  /* figure out how many occurences of target in the line */  
  do {
    /* set pointer to first occurence of target */ 
    q = strstr(q, target); 
    if (q) {
       q+= strlen(target); 
       count++; 
    }
  } while(q);
/* for each occurence of the target, copy over the substring that 
 * doesn't contain the target. Move the start_substr pointer so that
 * it skips the target for each copy. */
  for(i = 0; i < count; i++) {
    const char *end_substr = strstr(start_substr, target); 
    int substr_length = end_substr - start_substr;
    memcpy(result, start_substr, substr_length);
    result += substr_length; 
    start_substr = end_substr + strlen(target); 
  } 
  /* now that you've copied over the line without the target, add the remnants 
   * of the line */ 
  leftover = strlen(line) - (start_substr - line) + 1; 
  memcpy(result, start_substr, leftover); 
  return updated_line; 
}

/* data will will be loaded from a file instead of using an array.*/ 
int load_file(Document *doc, const char *filename) {
  FILE *input;
  char line[MAX_STR_SIZE + 1];
  int num_paragraphs = 0, line_index = 0, i;  
  if(doc == NULL || filename == NULL || (input = fopen(filename, "r")) == NULL) {
    return FAILURE;
  } else {
    /* read the entire file */
    while(fgets(line, MAX_STR_SIZE + 1, input) != NULL) {
      /* blank lines mark the start of new paragraph */
      if(isblank(line)) {
        if(line_index > 0) {
        add_paragraph_after(doc, num_paragraphs); 
        num_paragraphs++;
        line_index = 0; 
        } 
      } else {
        if(num_paragraphs == 0) {
          add_paragraph_after(doc, num_paragraphs);
          num_paragraphs++;
        }
      /* copy the line into the document */
      for(i = 0; line[i] != '\0'; i++) {
        if(line[i] == '\n') {
          line[i] = '\0';
          break;
        }
      } 
      strcpy(doc->paragraphs[num_paragraphs - 1].lines[line_index], line); 
      doc->paragraphs[num_paragraphs - 1].number_of_lines++;
      line_index++;
      }
    }
    fclose(input);
  }
  return SUCCESS;
}

int isblank(const char *line) {
  int i;

  for (i = 0; line[i] != '\0'; i++) {
    if (!isspace(line[i])) {
      return 0;
    }
  }
  return 1;
}

/* print the paragraphs associated with a document to the specified file 
 * (overwriting the file)*/
int save_document(Document *doc, const char *filename) {
  int i, j;
  FILE *file;

  if(doc == NULL || filename == NULL || fopen(filename, "r") == NULL) {
    return FAILURE;
  }
  if ((file = fopen(filename, "w")) == NULL) {
    return FAILURE;
  }

  /* loop through the document */
  for(i = 0; i < doc->number_of_paragraphs; i++) {
    for(j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      fprintf(file, "%s\n", doc->paragraphs[i].lines[j]);
    }
    fprintf(file, "\n");    
  }
  fclose(file);
  return SUCCESS;    
}
