#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Quiz Description";
   int paragraph_number, line_number;

   init_document(&doc, doc_name);

   /* Adding paragraph */
   paragraph_number = 0;
   add_paragraph_after(&doc, paragraph_number);

   /* Adding lines to paragraph */
   paragraph_number = 1;

   /* First line */
   line_number = 0;
   add_line_after(&doc, paragraph_number, line_number, "Quiz Information is:");

   /* Additional lines */
   add_line_after(&doc, paragraph_number, 1, "The quiz will");
   add_line_after(&doc, paragraph_number, 2, "cover topics discussed in lecture.");
   add_line_after(&doc, paragraph_number, 2, "only");

   print_document(&doc);

   return 0;
}
