#include <stdio.h>

typedef enum {
  OP_ADDX,
  OP_NOOP,
} op_type_t;

typedef struct {
  int value; 
} op_addx_t;

typedef struct {} op_noop_t;

typedef struct {
  op_type_t type;

  union {
    op_addx_t addx;
    op_noop_t noop; 
  };
} op_t;

static int is_digit(char c) {
  return c >= '0' && c <= '9';
}


// will not fail
static int 
parse_int(const char* str) {
  int ret = 0;
  int is_neg = (str[0] == '-') ? 1 : 0;

  if (is_neg) ++str;

  while(is_digit(*str)) {
    ret = (ret * 10) + ((*str) - '0');
    ++str;
  }

  return is_neg ? -ret : ret; 
}

static op_t
parse_op(const char* str) {
  op_t ret;
  // Cheap way to parse
  if (str[0] == 'a') {
    ret.type = OP_ADDX;
    ret.addx.value = parse_int(str + 5);
  }
  else if (str[0] == 'n') {
    ret.type = OP_NOOP;
  }
  return ret;
}


int main() {

  FILE* fp = fopen("input.txt", "r");
  char buffer[256];

  if (fp)
  {
    int x = 1;
    int cycle = 1;
    int is_done = 0;
    int is_busy = 0;
    int sum = 0;
    int line = 1;

    while(!is_done) 
    {
#if 1
      if ((cycle-20) % 40 == 0)  {
        printf("Cycle #%3d, %3d, %3d\n",cycle, x, cycle*x); 
        sum += cycle*x;
      }
#else 
      printf("Cycle #%3d, %3d, %3d\n",cycle, x, cycle*x); 
      sum += cycle*x;
#endif

      op_t current_op; 
      if (is_busy) {
        if (current_op.type == OP_ADDX) {
          x += current_op.addx.value;
          is_busy = 0;
        }
      }

      else {
        if(fgets(buffer, sizeof(buffer), fp))
        {
          current_op = parse_op(buffer);
          if (current_op.type == OP_ADDX) {
            is_busy = 1;
          }

#if 0
          printf("Line #%3d, %3d", line++, current_op.type);
          if (current_op.type == OP_ADDX) 
            printf(" %3d", current_op.addx.value);
          printf("\n");
#endif
        }
        else {
          is_done = 1;
        }
      }

      ++cycle;
    }
    printf("sum: %d\n", sum);
  }

}
