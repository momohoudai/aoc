#include <stdio.h>

#define CRT_W 40 
typedef enum {
  OP_NONE,
  OP_ADDX,
  OP_NOOP,
} op_type_t;

typedef struct {
  int value; 
} op_addx_t;

typedef struct {} op_noop_t;

typedef struct {
  op_type_t type;
  int life;
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
    ret.life = 2;
  }
  else if (str[0] == 'n') {
    ret.type = OP_NOOP;
    ret.life = 1;
  }
  return ret;
}


int main() {

  FILE* fp = fopen("input.txt", "r");
  char buffer[256];

  if (fp)
  {

    int prev_sprite_center = 0;
    int sprite_center = 1;
    int cycle = 1;

    int is_busy = 0;

    int line = 1;
    
    int pixel_counter = 0;

    op_t current_op = {0}; 
    while(1) 
    {

      // Read
      if(current_op.type == OP_NONE) 
      {
        if(fgets(buffer, sizeof(buffer), fp))
        {
          current_op = parse_op(buffer);

#if 0 
          printf("Line  #%d: %3d", line++, current_op.type);
          if (current_op.type == OP_ADDX) 
            printf(" %3d", current_op.addx.value);
          printf("\n");
#endif
        }
        else {
          break;
        }
      }
      

      // Draw
#if 0
      printf("Cycle #%d: Draw pixel at %d - ", cycle, pixel_counter);
#endif
      if (pixel_counter >= sprite_center -1 &&
          pixel_counter <= sprite_center +1)
      {
        printf("#");
      }
      else {
        printf(".");
      }
#if 0
      printf("\n");
#endif
      pixel_counter++;
      if (pixel_counter >= CRT_W) {
        printf("\n");
        pixel_counter = 0;
      }

      // Execute
      {
        --current_op.life;
        if (current_op.life <= 0) {
          if (current_op.type == OP_ADDX) {
            sprite_center += current_op.addx.value;
#if 0
            printf("Cycle #%d: sprite_center is %d\n", cycle, sprite_center);
#endif 
          }
          else if (current_op.type == OP_NOOP) {
            // Do nothing!
          }

          current_op.type = OP_NONE;
        }
      }
      ++cycle;
    }
  }

}
