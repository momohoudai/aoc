// https://adventofcode.com/2022/day/2

#include <stdio.h>


// We should really really do a nested switch case
// for this question but using numbers is fun.

int calc_score(int lhs, int rhs) {
  if (lhs == 0) {
    if (rhs == 0) return 3;
    else if (rhs == 1) return 0;
    else if (rhs == 2) return 6;
  }
  else if (lhs == 1) {
    if (rhs == 0) return 6;
    else if (rhs == 1) return 3;
    else if (rhs == 2) return 0;
  }
  else // (lhs == 2)
  {
    if (rhs == 0) return 0;
    else if (rhs == 1) return 6;
    else if (rhs == 2) return 3;
  }
  
  return 0;
}


int main() {
  

  int score = 0;
  FILE* fp = fopen("input.txt", "r");
  const int buffer_size = 64;
  char buffer[buffer_size];
  if (fp) {
    while(fgets(buffer, buffer_size, fp)) {
      int them = buffer[0] - 'A';

      // we reduce us to match them
      // i.e. X Y Z becomes A B C

      int us = buffer[2] - 'X'; 

      score += us + 1;
      
      // Draw condition is when we match
      //   e.g. A vs A, B vs B, C vs C
      // Win/Lose condition
      //   Since:
      //     A = Rock
      //     B = Paper
      //     C = Scissors
      //   Then:
      //     A lose to B lose to C lose to A
      //   
      //   
      int win_score = calc_score(us, them);
      printf("%c vs %c: %d + %d\n", them+'A', us+'A', us+1, win_score);
      score += win_score; 
      

      printf("Current score: %d\n", score);

    }

    printf("Total score: %d\n", score);


    fclose(fp);
  }


}
