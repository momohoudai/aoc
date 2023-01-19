
// https://adventofcode.com/2022/day/5

static u32_t 
d5a(const char* filename) 
{
  FILE* fp = fopen(filename, "r");
  if (fp) {
    char buffer[128];
    u32_t stack_count = 0; 
    u32_t original_highest = 0;

    // One pass to initialize data structure
    while(fgets(buffer, sizeof(buffer), fp)) {
      if ( buffer[0] == '\n' || buffer[0] == '\r' ) break;

      static u32_t first = 0;
      if (!first) {
        stack_count = cstr_count_readables(buffer)/3;  
        first = 1;
      }
      ++original_highest;    
    }
    fseek(fp, 0, SEEK_SET);
    original_highest -= 1;

    u32_t max_height = stack_count*original_highest;

    //printf("%d %d\n", stack_count, original_highest);

    char* stacks = (char*)calloc(stack_count * max_height + (stack_count * sizeof(u32_t)), 1);
    u32_t* stack_heights = (u32_t*)(stacks + stack_count * max_height);

#define stacks_at(i,j) (*(stacks+(i*max_height)+j))

    if (stacks) 
    {
      for (u32_t h = original_highest-1; h >= 0; --h) {
        fgets(buffer, sizeof(buffer), fp);
        for (u32_t i = 1, stack_index = 0; stack_index < stack_count ; i += 4, ++stack_index) {
          if (is_uppercase(buffer[i])) {
            //printf("oi: %c @ %d %d\n", buffer[i], stack_index, h);
            stacks_at(stack_index, h) = buffer[i];
            stack_heights[stack_index]++;

          }
        }

      }

#define stacks_print() \
      for (u32_t i = 0; i < stack_count ; ++i){ \
        printf("#%d: ", i+1); \
        for (u32_t j = 0; j < stack_heights[i]; ++j) { \
          printf("%c ", stacks_at(i,j)); \
        }\
        printf(" = %d\n", stack_heights[i]);\
      } 

      stacks_print();

      fgets(buffer, sizeof(buffer), fp);
      fgets(buffer, sizeof(buffer), fp);

#define stacks_pop(i) stacks_at(i, --stack_heights[i])
#define stacks_push(i, item) stacks_at(i, stack_heights[i]++) = (item)
#define stacks_last(i) stacks_at(i, stack_heights[i]-1)

      while(fgets(buffer, sizeof(buffer), fp)) {
        tokenizer_t t = { buffer, 0 }; 
        u32_t move_amount = tokenizer_get_next_u32(&t);
        u32_t from_index = tokenizer_get_next_u32(&t) - 1;
        u32_t to_index = tokenizer_get_next_u32(&t) - 1;
        printf("move %d from #%d to #%d\n", move_amount, from_index+1, to_index+1);
        for(u32_t move_index = 0; move_index < move_amount; ++move_index)
        {
          // pop from 'from'
          char popped = stacks_pop(from_index);
          stacks_push(to_index, popped);
        }
        stacks_print();
      }

      printf("answer: ");
      for(u32_t stack_index = 0; stack_index < stack_count; ++stack_index){
        printf("%c", stacks_last(stack_index));
      }
      printf("\n");

      free(stacks);
    }



    fclose(fp);
  }

  return 0;

}

static u32_t 
d5b(const char* filename) 
{
  FILE* fp = fopen(filename, "r");
  if (fp) {
    char buffer[128];
    u32_t stack_count = 0; 
    u32_t original_highest = 0;

    // One pass to initialize data structure
    while(fgets(buffer, sizeof(buffer), fp)) {
      if ( buffer[0] == '\n' || buffer[0] == '\r' ) break;

      static u32_t first = 0;
      if (!first) {
        stack_count = cstr_count_readables(buffer)/3;  
        first = 1;
      }
      ++original_highest;    
    }
    fseek(fp, 0, SEEK_SET);
    original_highest -= 1;

    u32_t max_height = stack_count*original_highest;

    printf("%d %d\n", stack_count, original_highest);

    char* stacks = (char*)calloc(stack_count * max_height + (stack_count * sizeof(u32_t)), 1);
    u32_t* stack_heights = (u32_t*)(stacks + stack_count * max_height);

#define stacks_at(i,j) (*(stacks+(i*max_height)+j))

    if (stacks) 
    {
      for (u32_t h = original_highest-1; h >= 0; --h) {
        fgets(buffer, sizeof(buffer), fp);
        for (u32_t i = 1, stack_index = 0; stack_index < stack_count ; i += 4, ++stack_index) {
          if (is_uppercase(buffer[i])) {
            //printf("oi: %c @ %d %d\n", buffer[i], stack_index, h);
            stacks_at(stack_index, h) = buffer[i];
            stack_heights[stack_index]++;

          }
        }

      }

#define stacks_print() \
      for (u32_t i = 0; i < stack_count ; ++i){ \
        printf("#%d: ", i+1); \
        for (u32_t j = 0; j < stack_heights[i]; ++j) { \
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
        u32_t move_amount = tokenizer_get_next_u32(&t);
        u32_t from_index = tokenizer_get_next_u32(&t) - 1;
        u32_t to_index = tokenizer_get_next_u32(&t) - 1;
        printf("move %d from #%d to #%d\n", move_amount, from_index+1, to_index+1);

        u32_t from_beg = stack_heights[from_index]-move_amount; 
        u32_t from_ope = stack_heights[from_index];

        u32_t to_beg = stack_heights[to_index]; 

        for(;from_beg < from_ope; ++from_beg){
          stacks_at(to_index, to_beg++) = stacks_at(from_index, from_beg);
        }
        stack_heights[from_index] -= move_amount;
        stack_heights[to_index] += move_amount;

        stacks_print();
      }

      printf("answer: ");
      for(u32_t stack_index = 0; stack_index < stack_count; ++stack_index){
        printf("%c", stacks_last(stack_index));
      }
      printf("\n");

      free(stacks);
    }

    fclose(fp);
  }

  return 0;

}
