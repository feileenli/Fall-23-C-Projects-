/* Feileen Li, 118924120, fli1234 */

#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/* Description here: This test checks initialize, print, and destroy  */
static int test1() {
   int days = 7;
   Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/* this test checks add event: adding the same event twice */
static int test2() {
   int days = 7;
   Calendar *calendar;

   if(init_calendar("January", days, comp_minutes, NULL, &calendar) == SUCCESS) {
     if(add_event(calendar, "test one", 1600, 100, NULL, 1) == SUCCESS) {
       if(add_event(calendar, "test one", 1600, 100, NULL, 1) == SUCCESS) {
         if(print_calendar(calendar, stdout, 1) == SUCCESS) {
           return destroy_calendar(calendar);
         }
       }
     }
   }
   destroy_calendar(calendar);
   return FAILURE;
}

/* this test checks add event: adding events that compare to 0 and printing 
 * when printall is 0 or false */
static int test3() {

  Calendar *calendar;
  int days = 4;

  if(init_calendar("February", days, comp_minutes, NULL, &calendar) == SUCCESS) {
     if(add_event(calendar, "test one", 1600, 100, NULL, 1) == SUCCESS) {
       if(add_event(calendar, "test two", 900, 120, NULL, 1) == SUCCESS) {
         if(add_event(calendar, "test three", 400, 130, NULL, 1) == SUCCESS) { 
           if(add_event(calendar, "TEST THREE", 1100, 130, NULL, 1) == SUCCESS) {
             if(print_calendar(calendar, stdout, 1) == SUCCESS) {
               return destroy_calendar(calendar);
             }  
           }
         }
       }
     }
   }
   return FAILURE;
}
/* this test checks both find event functions and if the event paramter is NULL */ 
static int test4() {
  Calendar *calendar;
  int days = 4;
  Event *event;

  if(init_calendar("MARCH", days, comp_minutes, NULL, &calendar) == SUCCESS) {
     if(add_event(calendar, "test one", 1600, 100, NULL, 1) == SUCCESS) {
       if(add_event(calendar, "test two", 900, 120, NULL, 1) == SUCCESS) {
         if(add_event(calendar, "test three", 400, 130, NULL, 1) == SUCCESS) {
           if(add_event(calendar, "test one", 1100, 140, NULL, 2) == SUCCESS) {
             if(find_event(calendar, "test three", &event) == SUCCESS) {
               printf("found event: %s\n", (event)->name);
               if(find_event_in_day(calendar, "test one", 2, &event) == SUCCESS) {
                 printf("found event: %s in day: %d\n", (event)->name, (event)->start_time); 
                 if(print_calendar(calendar, stdout, 1) == SUCCESS) {
                   return destroy_calendar(calendar);
                 }
               }
             }
           } 
         }
       }
     }
   }
   destroy_calendar(calendar);
   return FAILURE;
}

/* this test checks remove event and get event info before and after remove occurs*/
static int test5() {
  Calendar *calendar;
  int days = 4;
  char word[5] = "INFO";
  void *info;
  info = &word; 
 
  if(init_calendar("MARCH", days, comp_minutes, NULL, &calendar) == SUCCESS) {
     if(add_event(calendar, "test one", 1600, 100, NULL, 1) == SUCCESS) {
       if(add_event(calendar, "test two", 900, 120, info, 1) == SUCCESS) {
         if(add_event(calendar, "test three", 400, 130, NULL, 1) == SUCCESS) {
           if(add_event(calendar, "test five", 1100, 140, NULL, 2) == SUCCESS) {
             if(add_event(calendar, "test four", 1100, 130, NULL, 2) == SUCCESS) {
	       printf("test two info: %s\n", (char *)get_event_info(calendar, "test two")); 
               if(remove_event(calendar, "test two") == SUCCESS) {
                 printf("TEST TWO info: %s\n", (char *)get_event_info(calendar, "test two"));
                 if(remove_event(calendar, "test four") == SUCCESS) {
                   if(print_calendar(calendar, stdout, 1) == SUCCESS) {
                     return destroy_calendar(calendar); 
                   }
                 }
               } 
             }
           }
         }
       }
     }
   }
   return FAILURE;
}

/* this test checks clear_day and clear calendar*/
static int test6() {
  Calendar *calendar;
  int days = 4;

  if(init_calendar("MARCH", days, comp_minutes, NULL, &calendar) == SUCCESS) {
     if(add_event(calendar, "test one", 1600, 100, NULL, 1) == SUCCESS) {
       if(add_event(calendar, "test two", 900, 120, NULL, 1) == SUCCESS) {
         if(add_event(calendar, "test three", 400, 130, NULL, 1) == SUCCESS) {
           if(add_event(calendar, "test five", 1100, 140, NULL, 2) == SUCCESS) {
             if(add_event(calendar, "test four", 1100, 130, NULL, 2) == SUCCESS) {
               if(remove_event(calendar, "test two") == SUCCESS) {
                 if(clear_day(calendar, 2) == SUCCESS) {
                   if(print_calendar(calendar, stdout, 1) == SUCCESS) {
                     if(clear_calendar(calendar) == SUCCESS) {
                       print_calendar(calendar, stdout, 1);
                       return destroy_calendar(calendar);
                     }
                   }
                 }
               }
             }
           }
         }
       }
     }
   }
   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) {
     result = FAILURE;
     printf("test 1 failed\n");
   }
   if (test2() == FAILURE) {
     result = FAILURE;
     printf("test 2 failed\n");
   }
   if (test3() == FAILURE) {
     result = FAILURE;
     printf("test 3 failed\n");
   }
   if (test4() == FAILURE) {
     result = FAILURE;
     printf("test 4 failed\n");
   }
   if (test5() == FAILURE) {
     result = FAILURE;
     printf("test 5 failed\n");
   }
   if (test6() == FAILURE) {
     result = FAILURE;
     printf("test 6 failed\n");
   }

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
