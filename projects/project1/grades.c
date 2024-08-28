/* Feileen Li, 118924120, fli1234 */

#include <stdio.h> 
#include <math.h>
#include <limits.h>
#define LENGTH 200

/* function prototypes*/

double numeric_score(int array[], int num_of_dropped, int penalty_pnts, 
int size);
double mean (int array[], int num_of_assigned, int penalty_pnts);
double standard_deviation (int array[], int num_of_assigned, int penalty_pnts, 
double mean);
int processing(int array[], int num_of_assigned, int penalty_pnts, 
int num_of_dropped, char stats);

/*main function deals with input and output*/

int main() {
 
  int penalty_pnts, num_of_dropped, i, sum_weights = 0, num_of_assigned = 0, 
  database[LENGTH]; 
  char stats;

  /* input */
  scanf("%d %d %c %d", &penalty_pnts, &num_of_dropped, &stats, 
  &num_of_assigned);

  /* input one line of data for each assignment */ 
  for(i = 0; i < num_of_assigned; i++) {
   
    int assign_no, score, weight, days_late, index;
   
    scanf(" %d, %d, %d, %d", &assign_no, &score, &weight, &days_late);
    
    /* as each line of data is inputted, sort it into the array */
    index = (assign_no * 4) - 4;
    database[index] = assign_no;
    database[index+1] = score;
    database[index+2] =	weight;
    database[index+3] =	days_late;
    sum_weights += weight;
  }
  /* if the sum of weights doesn't equal 100 print an error message */
  if(sum_weights != 100) {
    printf("ERROR:Invalid values provided \n");
    return 0;
  }
  /* call processing to print out the output */
  processing(database, num_of_assigned, penalty_pnts, num_of_dropped, stats); 
  return 0;
 }

/* prints out the output */
int processing(int array[], int num_of_assigned, int penalty_pnts, 
int num_of_dropped, char stats) {
  
  int i;
  double score = numeric_score(array, num_of_dropped, penalty_pnts, 
  num_of_assigned);
  double mean_value = mean(array, num_of_assigned, penalty_pnts); 
  double stand_deviation = standard_deviation (array, num_of_assigned, 
  penalty_pnts, 
  mean_value); 
 
  printf("Numeric Score: %5.4f\n", score);
  printf("Points Penalty Per Day Late: %d\n", penalty_pnts);
  printf("Number of Assignments Dropped: %d\n", num_of_dropped); 
  printf("Values Provided:\nAssignment, Score, Weight, Days Late\n");
  
  /* prints the array of assignments and their data */ 
  for(i = 1; i <= num_of_assigned; i++) {
    int index = (i * 4) - 4;
    printf("%d, %d, %d, %d\n", array[index], array[index+1], array[index+2], 
    array[index+3]);
  }
  
  /* either Y or y will request stat information. Any other character will 
  * not */
  if(stats == 'Y' || stats == 'y') {
    printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean_value, 
    stand_deviation); 
  } 
  return 0;
}   

/* calculates the mean of all the assignment scores */
double mean (int array[], int num_of_assigned, int penalty_pnts) {
 
  int i;
  double mean = 0, sum = 0;
  
  /* find the total sum by adding each score to and subtracting the penalty 
  * points for each score */
  for(i = 1; i < num_of_assigned * 4; i+=4) { 
    int assignment_sum = array[i] - (array[i+2] * penalty_pnts);
    if (assignment_sum < 0) {
      assignment_sum = 0;
    }
    sum += assignment_sum; 
  }
  mean = sum / num_of_assigned;
  return mean;
}

/* calculates the standard deviation of all the assignment scores */
double standard_deviation (int array[], int num_of_assigned, int penalty_pnts, 
double mean) { 
  
  double stand_deviation, sum_of_diff = 0; 
  int i, j = 0, k, scores[LENGTH];

  /* make an array of all the scores after deducting late points */
  for(i = 1; i <= num_of_assigned; i++) {
    int index = ((i - 1) * 4) + 1;
    int score = array[index] - (array[index +2] * penalty_pnts);
    if(score < 0) {
      score = 0;
    }
    scores[j++] = score; 
  }
  /* for each score, calculate the difference between the score and the mean, 
  * square it, then add it to the sum of differences */
  for (k = 0; k < num_of_assigned; k++) {
    double diff = scores[k] - mean;
    sum_of_diff += diff * diff;
  }  
  /* standard deviation formula */
  stand_deviation = sqrt(sum_of_diff / num_of_assigned);
  return stand_deviation;
} 

/* calculate the numeric score */
double numeric_score(int array[], int num_of_dropped, int penalty_pnts, 
int size) {
  
  int i, j, count = 0, values[LENGTH] = {0}, copy[LENGTH];
  double total_score = 0.0, total_weight = 0.0, numeric_score;

  /* copy the array of input values over */
  for(i = 0; i < LENGTH; i++) {
    copy[i] = array[i];
  } 
  /*create an array of the values (score x weight)*/ 
  for(i = 1; i<= size; i++) {
    int index = ((i - 1) * 4) + 1; 
    int value = copy[index] * copy[index + 1]; 
    values[i] = value; 
  } 
  /* for n number of assignments that need to be dropped, find the assignment 
  * with the min value */
  for(j = 0; j < LENGTH && count < num_of_dropped; j++) {
    int min_Value = INT_MAX;
    int min_Index = -1;

    for(i = 1; i <= size; i++) {
      if(values[i] < min_Value) {
        min_Value = values[i];
        min_Index = i;      
     } 
    }
    /* set the most recently found min value to a large number so we can find
    * the next min value */
    values[min_Index] = INT_MAX;
    /* set the weight for the assignment with the min value to 0 */
    copy[((min_Index - 1) * 4) + 2] = 0;
    count++;
  }
  /* using the updated array, calculate total_weight and total_score*/ 
  for(i = 1; i <= size; i++) {
    int index = ((i-1) * 4) + 1; 
    double score = copy[index] - (copy[index+2] * penalty_pnts);
    double weight = copy[index+1];
    if(score < 0) {
      score = 0.0;
    }
    total_weight += weight;
    total_score += (score * weight); 
  } 
  numeric_score = (total_score / total_weight); 
  return numeric_score; 
}
