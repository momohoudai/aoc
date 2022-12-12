// https://adventofcode.com/2022/day/1

#include <stdio.h>


int str_to_int(const char* str) {
  int ret = 0;
  while(*str >= '0' && *str <= '9') {
    ret *= 10;
    ret += (*str - '0'); 
    ++str; 
  }


  return ret;

}

int main() {

  int max = 0;
  int sum = 0;
  FILE* fp = fopen("input.txt", "r");
  const int buffer_size = 64;
  char buffer[buffer_size];
  if (fp) {
    while(fgets(buffer, buffer_size, fp)) {
      if (buffer[0] == '\n' || buffer[0] == '\r') {
        printf("sum of elf: %d\n", sum); 
        if (sum > max) {
          printf("MAX DETECTED!\n");
          max = sum;
        }
        sum = 0;
      }
      else {
        sum += str_to_int(buffer);
        printf("sum is now: %d\n", sum); 
      }
    }

    printf("fattest elf: %d\n", max);


    fclose(fp);
  }




}
