// https://adventofcode.com/2022/day/2

// We should really really do a nested switch case
// for this question but using numbers is fun.
static u32_t 
d2_calc_score(u32_t lhs, int rhs) {
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

static u32_t
d2a(const char* filename) 
{
  u32_t score = 0;
  FILE* fp = fopen(filename, "r");
  const u32_t buffer_size = 64;
  char buffer[buffer_size];
  if (fp) {
    while(fgets(buffer, buffer_size, fp)) {
      u32_t them = buffer[0] - 'A';

      // we reduce us to match them
      // i.e. X Y Z becomes A B C

      u32_t us = buffer[2] - 'X'; 

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
      u32_t win_score = d2_calc_score(us, them);
      //printf("%c vs %c: %d + %d\n", them+'A', us+'A', us+1, win_score);
      score += win_score; 
      //printf("Current score: %d\n", score);
    }

    //printf("Total score: %d\n", score);


    fclose(fp);
  }

  return score;

}


static u32_t 
d2b(const char* filename) 
{
  u32_t score = 0;
  FILE* fp = fopen(filename, "r");
  const u32_t buffer_size = 64;
  char buffer[buffer_size];
  if (fp) {
    while(fgets(buffer, buffer_size, fp)) {
      u32_t them = buffer[0] - 'A';

      // we reduce us to match them
      // i.e. X Y Z becomes A B C
      u32_t condition = buffer[2];

      u32_t us = 0;
      switch(condition) {
        case 'X': {
          // Need to lose
          us = them - 1;
          if (us == -1) us = 2;
        } break;
        case 'Y': {
          // Need to draw
          us = them;
        } break;
        case 'Z': {
          // Need to win
          us = them + 1;
          if (us == 3) us = 0;
        } break;


      }

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
      u32_t win_score = d2_calc_score(us, them);
      //printf("%c vs %c: %d + %d\n", them+'A', us+'A', us+1, win_score);
      score += win_score; 
      

      //printf("Current score: %d\n", score);

    }

    //printf("Total score: %d\n", score);


    fclose(fp);
  }

  return score;


}
