#include <stdio.h>
#include <stdlib.h>
// https://adventofcode.com/2022/day/5

// Crate is a char
//
typedef struct tokenizer_t {
  char* buffer;
  int at;
} tokenizer_t;

int is_digit(char c) {
  return c >= '0' &&  
         c <= '9';
}

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

int is_readable(char c) {
  return c != '\r' && c != '\n' && c != 0;
}

int is_uppercase(char c) {
  return c >= 'A' && c <= 'Z';
}


int count_readables(const char* buffer) {
  int count = 0;
  while(is_readable(*buffer++))
    count++;

  return count;
}



int main() {

  FILE* fp = fopen("input.txt", "r");
  if (fp) {
    char buffer[128];
    int stack_count = 0; 
    int original_highest = 0;

    // One pass to initialize data structure
    while(fgets(buffer, sizeof(buffer), fp)) {
      if ( buffer[0] == '\n' || buffer[0] == '\r' ) break;

      static int first = 0;
      if (!first) {
        stack_count = count_readables(buffer)/3;  
        first = 1;
      }
      ++original_highest;    
    }
    fseek(fp, 0, SEEK_SET);
    original_highest -= 1;

    int max_height = stack_count*original_highest;

    printf("%d %d\n", stack_count, original_highest);

    char* stacks = (char*)calloc(stack_count * max_height + (stack_count * sizeof(int)), 1);
    int* stack_heights = (int*)(stacks + stack_count * max_height);

#define stacks_at(i,j) (*(stacks+(i*max_height)+j))

    if (stacks) 
    {
      for (int h = original_highest-1; h >= 0; --h) {
        fgets(buffer, sizeof(buffer), fp);
        for (int i = 1, stack_index = 0; stack_index < stack_count ; i += 4, ++stack_index) {
          if (is_uppercase(buffer[i])) {
            //printf("oi: %c @ %d %d\n", buffer[i], stack_index, h);
            stacks_at(stack_index, h) = buffer[i];
            stack_heights[stack_index]++;

          }
        }

      }

#define stacks_print() \
      for (int i = 0; i < stack_count ; ++i){ \
        printf("#%d: ", i+1); \
        for (int j = 0; j < stack_heights[i]; ++j) { \
          printf("%c ", stacks_at(i,j)); \
        }\
        printf(" = %d\n", stack_heights[i]);\
      } 

      stacks_print();

      fgets(buffer, sizeof(buffer), fp);
      fgets(buffer, sizeof(buffer), fp);

#define stacks_last(i) stacks_at(i, stack_heights[i]-1)


      while(fgets(buffer, sizeof(buffer), fp)) {
        tokenizer_t t = { buffer, 0 }; 
        int move_amount = get_next_number(&t);
        int from_index = get_next_number(&t) - 1;
        int to_index = get_next_number(&t) - 1;
        printf("move %d from #%d to #%d\n", move_amount, from_index+1, to_index+1);

        int from_beg = stack_heights[from_index]-move_amount; 
        int from_ope = stack_heights[from_index];

        int to_beg = stack_heights[to_index]; 

        for(;from_beg < from_ope; ++from_beg){
          stacks_at(to_index, to_beg++) = stacks_at(from_index, from_beg);
        }
        stack_heights[from_index] -= move_amount;
        stack_heights[to_index] += move_amount;

        stacks_print();
      }

      printf("answer: ");
      for(int stack_index = 0; stack_index < stack_count; ++stack_index){
        printf("%c", stacks_last(stack_index));
      }
      printf("\n");

      free(stacks);
    }



    fclose(fp);
  }


}

