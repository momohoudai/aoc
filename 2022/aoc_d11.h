typedef struct d11_item_t {
  u32_t worry;
  struct d11_item_t* next;
  struct d11_item_t* prev;
} d11_item_t;

typedef enum {
  OP_TYPE_ADD,
  OP_TYPE_MUL,
  OP_TYPE_SQ,
} d11_op_type_t;


typedef struct {
  u32_t inspect_count;

  // item linked list
  d11_item_t* first;
  d11_item_t* last;

   // test 
  u32_t test_val;
  u32_t test_pass_monkey_index;
  u32_t test_fail_monkey_index;

  // operation 
  d11_op_type_t op_type; 
  u32_t op_val;

} d11_monkey_t;


static void 
d11_push_item(d11_monkey_t* m, d11_item_t* item) {
  if (m->last) {
    m->last->next = item;
    m->last = item;
  }
  else {
    m->first = m->last = item;
    item->next = 0;
  }
}

static d11_item_t* 
d11_pop_item(d11_monkey_t* m) {
  d11_item_t* ret = m->first;
  m->first = m->first->next;
  if (m->first == 0) m->last = 0; 
  ret->next = 0;
  return ret;
}

static s32_t 
d11_get_monkey_business(d11_monkey_t* monkeys, u32_t monkey_count) 
{
  u32_t m1 = monkey_count;
  u32_t m2 = monkey_count;


  for (u32_t i = 0; i < monkey_count; ++i) {
    if (m1 == monkey_count || monkeys[i].inspect_count > monkeys[m1].inspect_count) 
      m1 = i;
  }

  for (u32_t i = 0; i < monkey_count; ++i) {
    if (i == m1) continue;  
    if (m2 == monkey_count || monkeys[i].inspect_count > monkeys[m2].inspect_count) 
      m2 = i;
  }


  return monkeys[m1].inspect_count * monkeys[m2].inspect_count;

}

static void 
d11_print_monkeys(d11_monkey_t* monkeys, s32_t monkey_count) {
  for (s32_t i = 0; i < monkey_count; ++i) {
    d11_item_t *itr = monkeys[i].first;
    printf("Monkey #%d @ %3d: ", i, monkeys[i].inspect_count);
    while(itr != 0) {
      printf("%d ", itr->worry);
      itr = itr->next;
    }
    printf("\n");
  }

}

// only positive number. -1 is fail
static u32_t 
d11_parse_u32(const char* buffer, u32_t* at) {
  while(!is_digit(buffer[*at])) 
  {
    if (buffer[*at] == 0) 
    {
      return -1;
    }
    ++(*at);
  }

  u32_t num = 0;
  while(is_digit(buffer[*at])) {
    num *= 10;
    num += buffer[*at] - '0'; 
    (*at)++;
  }
  return num;

}

static void
d11a(const char* filename) {
  FILE* fp = fopen(filename, "r");
  if (fp) {
    s32_t item_count = 0;
    d11_item_t* items = 0;

    s32_t monkey_count = 0;
    d11_monkey_t* monkeys = 0;

    char buffer[128];
    while(fgets(buffer, array_count(buffer), fp))
    {
      if (buffer[0] == 'M') ++monkey_count;
      if (buffer[2] == 'S') 
      {
        // count the amount of commas
        for (char* it = buffer; *it != 0; ++it) {
          if (*it == ',') ++item_count;
        }
        ++item_count;
      }
    }
    //printf("monkeys: %d\nitems: %d\n", monkey_count, item_count);
    monkeys = calloc(monkey_count, sizeof(d11_monkey_t));
    items = calloc(item_count, sizeof(d11_item_t));
    if (monkeys && items) 
    {
      fseek(fp, 0, SEEK_SET);

      s32_t monkey_index = -1;
      s32_t item_index = 0;
      while(fgets(buffer, array_count(buffer), fp)) {
        if (buffer[0] == 'M') ++monkey_index;
        else if (buffer[2] == 'S') {
          u32_t at = 0;
          while(1) {
            u32_t worry = d11_parse_u32(buffer, &at);
            if (worry == -1) break;
            d11_item_t* item = items + item_index++;
            item->worry = worry;
            d11_push_item(monkeys + monkey_index, item);
            //printf("%d ", worry);
          }
          //printf("\n");
        }
        else if (buffer[2] == 'O') 
        {
          if (buffer[23] == '+') {
            monkeys[monkey_index].op_type = OP_TYPE_ADD;  
            s32_t at = 24;
            monkeys[monkey_index].op_val = d11_parse_u32(buffer, &at);
          }
          else if (buffer[23] == '*') {
            if (buffer[25] == 'o') {
              monkeys[monkey_index].op_type = OP_TYPE_SQ;  
            }
            else {
              s32_t at = 24;
              monkeys[monkey_index].op_type = OP_TYPE_MUL;  
              monkeys[monkey_index].op_val = d11_parse_u32(buffer, &at);
            }
          }
        }
        else if (buffer[2] == 'T') {
          s32_t at = 21;
          monkeys[monkey_index].test_val = d11_parse_u32(buffer, &at);
        }

        else if (buffer[7] == 't') {
          s32_t at = 29; 
          monkeys[monkey_index].test_pass_monkey_index = d11_parse_u32(buffer, &at);
        }
        else if (buffer[7] == 'f') {
          s32_t at = 30; 
          monkeys[monkey_index].test_fail_monkey_index = d11_parse_u32(buffer, &at);
        }
      }

#if 0
      // print monkey status
      for (s32_t i = 0; i < monkey_count; ++i) {
        d11_monkey_t* monkey = monkeys + i;
        printf("Monkey #%d\n", i);
        printf("  Starting items: ");
        for (d11_item_t* it = monkey->first; 
             it != 0; 
             it = it->next)
        {
          printf("%d ", it->worry);
        }
        printf("\n");
        printf("  Operation: new = old ");
        printf("%s", monkey->op_type == OP_TYPE_ADD ? "+ " : "* ");
        if (monkey->op_type == OP_TYPE_SQ) {
          printf("old");
        }
        else {
          printf("%d", monkey->op_val);
        }
        printf("\n");
        printf("  Test: divisible by %d\n", monkey->test_val);
        printf("    If true: throw to monkey %d\n", monkey->test_pass_monkey_index);
        printf("    If false: throw to monkey %d\n", monkey->test_fail_monkey_index);
      }
#endif
      

      //printf("=== Round 0 ===\n");
      //d11_print_monkeys(monkeys, monkey_count);
      for (s32_t round = 1; round <= 20; ++round) 
      {
        for(s32_t monkey_index = 0; 
            monkey_index < monkey_count; 
            ++monkey_index)
        {
          // inspect each item
          d11_monkey_t* monkey = monkeys + monkey_index;
#if 1
          while(monkey->first) 
          {
            // monkey inspects item 
            if(monkey->op_type == OP_TYPE_ADD) {
              monkey->first->worry += monkey->op_val;
            }
            else if(monkey->op_type == OP_TYPE_MUL) {
              monkey->first->worry *= monkey->op_val;
            }
            else if(monkey->op_type == OP_TYPE_SQ) {
              monkey->first->worry *= monkey->first->worry;
            }

            monkey->first->worry /= 3;

            s32_t d11_monkey_to_throw = 0;
            if ((monkey->first->worry % monkey->test_val) == 0) { 
              d11_monkey_to_throw = monkey->test_pass_monkey_index;
            }
            else {
              d11_monkey_to_throw = monkey->test_fail_monkey_index; 
            }

            //printf("Monkey #%d: Throwing %d to %d\n", monkey_index, monkey->first->worry,  d11_monkey_to_throw);

            d11_item_t* item = d11_pop_item(monkey);
            d11_push_item(monkeys + d11_monkey_to_throw, item);


            monkey->inspect_count++;
          }

#endif 
        }

        //printf("=== Round %d ===\n", round);
        //d11_print_monkeys(monkeys, monkey_count);

      }

      printf("%d", d11_get_monkey_business(monkeys, monkey_count));





    }

    if (monkeys) free(monkeys);
    if (items)   free(items);

    fclose(fp);
  }
}

