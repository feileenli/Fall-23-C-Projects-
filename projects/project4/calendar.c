/* Feileen Li, 118924120, fli1234 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "calendar.h"
#include "event.h"

/* This function initializes a Calendar structure based on the parameters 
 * provided. */
int init_calendar(const char *name, int days,
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar){

   Calendar *cal = malloc(sizeof(Calendar));
  /* The function will fail and return FAILURE if calendar and/or name are NULL, 
   * if the number of days is less than 1 */
   if(calendar == NULL || name == NULL || days < 1) {
     return FAILURE;
   }
   /* memory allocation */
   if(cal) {
     cal->name = malloc(strlen(name) + 1); 
     if(cal->name) {
       strcpy(cal->name, name);
       cal->events = calloc(days, sizeof(Event *)); 
       if(cal->events) {
         cal->days = days;
         cal->total_events = 0;
         cal->comp_func = comp_func; 
         cal->free_info_func = free_info_func; 
         *calendar = cal; 
         return SUCCESS;
       }
     }
   }
   return FAILURE;
}

/* This function prints, to the designated output stream, the calendar’s name, 
 * days, and total number of events if print all is true; otherwise this 
 * information will not be printed */
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {

   int i, count = 0;
  /* This function will fail and return FAILURE if calendar and/or output stream 
   * are NULL */
   if(calendar == NULL || output_stream == NULL) {
     return FAILURE;
   }
   if(print_all) {
     fprintf(output_stream, "Calendar's Name: \"%s\"\nDays: %d\nTotal Events: %d\n", 
     calendar->name, calendar->days, calendar->total_events);
   }
   /* Information about each event (name, start time, and duration) will be 
    * printed regardless the value of print all. */
   printf("\n**** Events ****\n");
   if(calendar->total_events == 0) {
     return SUCCESS;
   }
   for(i = 0; i < calendar->days; i++) {
     Event *curr_event = calendar->events[i];
     printf("Day %d\n", i + 1);

     while(count < calendar->total_events && curr_event != NULL) {
       printf("Event's name: \"%s\", Start_time: %d, Duration: %d\n",
       curr_event->name, curr_event->start_time, curr_event->duration_minutes); 
       curr_event = curr_event->next;  
       count++;
     }
   }
   return SUCCESS;
}

/* This function adds the specified event to the list associated with the day 
 * parameter. */
int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day)  {
    Event *event, **event_list, *current, *new_event, *prev_event;
 /* This function will fail and return FAILURE if calendar and/or name are NULL, 
  * start time is invalid (must be a value between 0 and 2400, inclusive), 
  * duration minutes is less than or equal to 0, day is less than 1 or greater 
  * than the number of calendar days, if the event already exist, or if any memory 
  * allocation fails. */ 
  if(calendar == NULL || name == NULL || start_time > 2400 || start_time < 0 || duration_minutes
     <= 0 || calendar->days < day || day < 1) {
    return FAILURE;
  }
  if(find_event(calendar, name, &event) == SUCCESS) {
    return FAILURE;
  } 
 /* The function must allocate memory for the new event and for the event’s name. 
  * Other fields of the event structure will be initialized based on the parameter 
  * values. */
  event_list = &(calendar->events[day - 1]);
  current = *event_list; 
  prev_event = NULL;
  new_event = malloc(sizeof(Event)); 
  if(new_event == NULL) {
    return FAILURE;
  }
  new_event->name = malloc(strlen(name) + 1);
  if(new_event->name == NULL) {
    free(new_event);
    return FAILURE;
  }
  strcpy(new_event->name, name);
  new_event->start_time = start_time;
  new_event->duration_minutes = duration_minutes;
  new_event->info = info; 
  new_event->next = NULL; 
 
  while(current != NULL && calendar->comp_func(new_event, current) > 0) {
   prev_event = current;
   current = current->next; 
  }
  new_event->next = current;
  if(prev_event == NULL) {
    *event_list = new_event; 
  } else {
    prev_event->next = new_event; 
  }
  calendar->total_events++;
  return SUCCESS; 
}

/* This function will return a pointer (via the out parameter event) to the 
 * event with the specified name (if any). */
int find_event(Calendar *calendar, const char *name, Event **event)  {

  int i;
 /* The function will fail and return FAILURE if calendar and/or name are NULL. */ 
  if(calendar == NULL || name == NULL) {
    return FAILURE;
  }
 /* If the event parameter is NULL, no pointer will be returned. Notice the out 
  * parameter event should not be modified unless the event is found. */
  for(i = 0; i < calendar->days; i++) {
    Event *curr_event = calendar->events[i]; 
     while(curr_event != NULL && strcmp(curr_event->name, name) != 0) {
       curr_event = curr_event->next;          
     }
     if(curr_event != NULL) {
       if(event != NULL) {
         *event = curr_event;
       } 
       return SUCCESS;
     }
  }
  return FAILURE;
}

/* This function will return a pointer (via the out parameter event) to the event 
 * with the specified name in the specified day (if such event exist). */
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event)  {

  Event *curr_event;
 /* The function will fail and return FAILURE if calendar and/or name are NULL, or 
  * if the day parameter is less than 1 or greater than the number of calendar days. */ 
  if(calendar == NULL || name == NULL || day < 1 || day > calendar->days) {
    return FAILURE;
  }
  curr_event = calendar->events[day-1];  
  while(curr_event != NULL && strcmp(curr_event->name, name) != 0) {
    curr_event = curr_event->next;  
  }
  if(curr_event != NULL) {
    if(event != NULL) {
      *event = curr_event;
    }  
    return SUCCESS;
  }
  return FAILURE;
}

/* This function will remove the specified event from the calendar returning 
 * any memory allocated for the event. */
int remove_event(Calendar *calendar, const char *name)  {

  int i;  
  Event *event, **event_list, *current, *prev; 

  /* This function will fail and return FAILURE if calendar and/or name are 
   * NULL or if the event cannot be found */
  if(calendar == NULL || name == NULL) {
    return FAILURE;
  }   
  if(find_event(calendar, name, &event) == SUCCESS) {
    for(i = 0; i < calendar->days; i++) {
      event_list = &(calendar->events[i]);
      current = *event_list; 
      prev = NULL; 
      while(current != NULL && strcmp(name, current->name) != 0) {
        prev = current;
        current = current->next;
      }
      if(current != NULL) {
        if(prev == NULL) {
          *event_list = current->next; 
        } else {
          prev->next = current->next; 
        }
        if(calendar->free_info_func && current->info != NULL) {
          calendar->free_info_func(current->info); 
        }
        free(current->name);
        free(current);
        calendar->total_events--; 
        return SUCCESS;
      }
    }
  }
  return FAILURE;        
}

/* This function returns the info pointer associated with the specified event.*/  
void *get_event_info(Calendar *calendar, const char *name)  {
  Event *event;
  
  if(find_event(calendar, name, &event) == SUCCESS) {
    return event->info; 
  }
  return NULL;

}

/* This function will remove all the event lists associated with the calendar and 
 * set them to empty lists. */
int clear_calendar(Calendar *calendar) {

  int i;
 /* This function will fail and return FAILURE if calendar is NULL */
  if(calendar == NULL) {
    return FAILURE;
  } 
  for(i = 1; i <= calendar->days; i++) {
    clear_day(calendar, i); 
  }
  calendar->total_events = 0;
  return SUCCESS;
}

/* This function will remove all the events for the specified day setting the 
 * event list to empty. */
int clear_day(Calendar *calendar, int day) {
  
   Event *event_list, *current_event;
  /* This function will fail and return FAILURE if calendar is NULL, or if the 
   * day is less than 1 or greater than the calendar days */
   if(calendar == NULL || day < 1 || day > calendar->days) {
    return FAILURE;
   }
   event_list = (calendar->events[day-1]); 
   while(event_list != NULL) {
     current_event = event_list; 
     event_list = event_list->next;
     remove_event(calendar, current_event->name); 
   }
  return SUCCESS;
}

/* This function will return memory that was dynamically-allocated for the calendar.*/
int destroy_calendar(Calendar *calendar) {

   int i, count = 0;
  /* This function will fail and return FAILURE if calendar is NULL */
   if(calendar == NULL) {
     return FAILURE;
   }
   /* If an event has an info field other than NULL and a free info func exists, 
    * the function will be called on the info field.*/
    for(i = 0; i < calendar->days; i++) {
     Event **curr_event = &(calendar->events[i]);

     while(count < calendar->total_events && *curr_event != NULL) {
       Event *temp = *curr_event; 
       if(temp->info && calendar->free_info_func) {
         calendar->free_info_func(temp->info); 
       }
       *curr_event = (*curr_event)->next; 
       free(temp->name);
       free(temp); 
       count++;
     }
   }
   free(calendar->name); 
   free(calendar->events);
   free(calendar); 
   return SUCCESS; 
}

