#include <stdio.h>
#include <string.h>
// https://adventofcode.com/2022/day/3

#if 0
int str_len(const char* str ) {
  // common strlen that counts what I care
  int ret = 0;
  while(*str >= 'a' && *str <= 'z' ||
        *str >= 'A' && *str <= 'Z') 
  {
    ++str;
    ++ret;
  }

  return ret;
  
}
#endif

int is_valid_char(char c) {
  return c >= 'a' && c <= 'z' ||
         c >= 'A' && c <= 'Z';
}

int get_priority(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a' + 1;
  }
  if (c >= 'A' && c <= 'Z') {
    return c - 'A' + 27;
  }
  return 0;
}


int main() {

  int score = 0;
  FILE* fp = fopen("input.txt", "r");
  char buffer[3][128];

  if (fp) {
    int sum = 0;
    int rucksack_index = 0;
    int priority = 0;
    int is_done = 0;
    // NOTE(momo): There are exactly 300 lines
    while(1) {
      char common = 0;
      // Fill buffers
      for (int i = 0; i < 3; ++i)
      {
        if(!fgets(buffer[i], sizeof(buffer[i]), fp)) {
          goto finished;
        }
      }

      // search for common shit between all 3 bags
      for (char* it0 = buffer[0]; is_valid_char(*it0); ++it0)
      {
        for(char* it1 = buffer[1]; is_valid_char(*it1); ++it1)
        {
          for(char* it2 = buffer[2]; is_valid_char(*it2); ++it2)
          {
            if (*it0 == *it1 && *it1 == *it2) {
              common = *it0;
              goto found;
            }

          }

        }
      }

found:
      sum += get_priority(common); 

    }

finished:

    printf("sum is %d\n", sum);
    fclose(fp);
  }


}

