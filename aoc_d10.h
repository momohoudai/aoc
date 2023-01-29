typedef enum {
  D10_OP_NONE,
  D10_OP_ADDX,
  D10_OP_NOOP,
} d10_op_type_t;

typedef struct {
  int value; 
} d10_op_addx_t;

typedef struct {} d10_op_noop_t;

typedef struct {
  d10_op_type_t type;
  u32_t life;
  union {
    d10_op_addx_t addx;
    d10_op_noop_t noop; 
  };
} d10_op_t;


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

static d10_op_t
parse_op(const char* str) {
  d10_op_t ret;
  // Cheap way to parse
  if (str[0] == 'a') {
    ret.type = D10_OP_ADDX;
    ret.addx.value = parse_int(str + 5);
    ret.life = 2;
  }
  else if (str[0] == 'n') {
    ret.type = D10_OP_NOOP;
    ret.life = 1;
  }
  return ret;
}

static void
d10a(const char* filename) {

  FILE* fp = fopen(filename, "r");
  char buffer[256];

  if (fp)
  {
    u32_t x = 1;
    u32_t cycle = 1;
    b32_t is_done = 0;
    b32_t is_busy = 0;
    u32_t sum = 0;
    u32_t line = 1;

    while(!is_done) 
    {
#if 1
      if ((cycle-20) % 40 == 0)  {
        //printf("Cycle #%3d, %3d, %3d\n",cycle, x, cycle*x); 
        sum += cycle*x;
      }
#else 
      printf("Cycle #%3d, %3d, %3d\n",cycle, x, cycle*x); 
      sum += cycle*x;
#endif

      d10_op_t current_op; 
      if (is_busy) {
        if (current_op.type == D10_OP_ADDX) {
          x += current_op.addx.value;
          is_busy = 0;
        }
      }

      else {
        if(fgets(buffer, sizeof(buffer), fp))
        {
          current_op = parse_op(buffer);
          if (current_op.type == D10_OP_ADDX) {
            is_busy = 1;
          }

#if 0
          printf("Line #%3d, %3d", line++, current_op.type);
          if (current_op.type == D10_OP_ADDX) 
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
    printf("%d", sum);
  }

}

static void
d10b(const char* filename) {
#define CRT_W 40 
  FILE* fp = fopen(filename, "r");
  char buffer[256];

  if (fp)
  {
    printf("\n");

    s32_t prev_sprite_center = 0;
    s32_t sprite_center = 1;
    s32_t cycle = 1;
    b32_t is_busy = false;
    s32_t line = 1;
    
    s32_t pixel_counter = 0;

    d10_op_t current_op = {0}; 
    while(1) 
    {

      // Read
      if(current_op.type == D10_OP_NONE) 
      {
        if(fgets(buffer, sizeof(buffer), fp))
        {
          current_op = parse_op(buffer);

#if 0 
          printf("Line  #%d: %3d", line++, current_op.type);
          if (current_op.type == D10_OP_ADDX) 
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
          if (current_op.type == D10_OP_ADDX) {
            sprite_center += current_op.addx.value;
#if 0
            printf("Cycle #%d: sprite_center is %d\n", cycle, sprite_center);
#endif 
          }
          else if (current_op.type == D10_OP_NOOP) {
            // Do nothing!
          }

          current_op.type = D10_OP_NONE;
        }
      }
      ++cycle;
    }
  }

#undef CRT_W
}
