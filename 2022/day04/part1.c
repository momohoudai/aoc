#include <stdio.h>
#include <string.h>
// https://adventofcode.com/2022/day/4

typedef struct range_t {
  int min, max;
}range_t;

int is_fully_overlapping(range_t lhs, range_t rhs) {
  int is_lhs_overlapping = lhs.min <= rhs.min && lhs.max >= rhs.max;
  int is_rhs_overlapping = rhs.min <= lhs.min && rhs.max >= lhs.max; 

  return is_lhs_overlapping || is_rhs_overlapping;
}

int is_digit(char c) {
  return c >= '0' &&  
         c <= '9';
}

typedef struct tokenizer_t {
  char* buffer;
  int at;
} tokenizer_t;

// terrible algorithm that will break if input is bad
// in the case of this qn, do NOT call more than 4 times
int get_next_number(tokenizer_t* t) {
  // go forward until we find a digit
  while(!is_digit(t->buffer[t->at])) ++(t->at);

  int num = 0;
  while(is_digit(t->buffer[t->at])) {
    num *= 10;
    num += t->buffer[t->at] - '0'; 
    t->at++;
  }
  return num;
}

int main() {

  int score = 0;
  FILE* fp = fopen("input.txt", "r");
  char buffer[16];
  if (fp) {

    int overlapping_pairs =  0;; 
    
    while(fgets(buffer, sizeof(buffer), fp)) {
      tokenizer_t tokenizer = { buffer, 0 };
      range_t first_elf;
      range_t second_elf;

      first_elf.min = get_next_number(&tokenizer);
      first_elf.max = get_next_number(&tokenizer);

      second_elf.min = get_next_number(&tokenizer);
      second_elf.max = get_next_number(&tokenizer);

      int overlap = is_fully_overlapping(first_elf, second_elf);
      printf("%d-%d,%d-%d: %d\n", 
             first_elf.min, first_elf.max, 
             second_elf.min, second_elf.max, 
             overlap);

      overlapping_pairs += overlap;


    }

    printf("Overlapping pairs: %d\n", overlapping_pairs);
    fclose(fp);
  }


}

