#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Loading Document ";
   int data_lines = 8;
   char data1[20][MAX_STR_SIZE + 1] = {
        "The first course you need to take",
	"is cmsc131.  This course will be",
	"followed by cmsc132 (which is also based on Java).",
	"",
	"The next course you will take is cmsc216.",
	"This course relies on C.",
	"",
	"Ruby and Ocaml will be covered in cmsc330"
   };
   char data2[20][MAX_STR_SIZE + 1] = {
        "When you start taking",
	"400-level courses you can specialize in an area",
	"",
	"There are courses in",
	"databases, networking etc."
   };
   char data3[20][MAX_STR_SIZE + 1] = {
        "600-level courses",
	"are graduate courses"
   };

   init_document(&doc, doc_name);
   
   data_lines = 8;
   load_document(&doc, data1, data_lines);

   data_lines = 5;
   load_document(&doc, data2, data_lines);

   data_lines = 2;
   load_document(&doc, data3, data_lines);

   print_document(&doc);

   return 0;
}
