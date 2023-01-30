typedef struct item_t {
  int worry;
  struct item_t* next;
  struct item_t* prev;
} item_t;

typedef enum {
  OP_TYPE_ADD,
  OP_TYPE_MUL,
  OP_TYPE_SQ,
} op_type_t;


typedef struct {
  int inspect_count;

  // item linked list
  item_t* first;
  item_t* last;

   // test 
  int test_val;
  int test_pass_monkey_index;
  int test_fail_monkey_index;

  // operation 
  op_type_t op_type; 
  int op_val;

} monkey_t;


static void push_item(monkey_t* m, item_t* item) {
  if (m->last) {
    m->last->next = item;
    m->last = item;
  }
  else {
    m->first = m->last = item;
    item->next = 0;
  }
}

static item_t* pop_item(monkey_t* m) {
  item_t* ret = m->first;
  m->first = m->first->next;
  if (m->first == 0) m->last = 0; 
  ret->next = 0;
  return ret;
}

static int 
get_monkey_business(monkey_t* monkeys, int monkey_count) 
{
  int m1 = -1;
  int m2 = -1;


  for (int i = 0; i < monkey_count; ++i) {
    if (m1 == -1 || monkeys[i].inspect_count > monkeys[m1].inspect_count) 
      m1 = i;
  }

  for (int i = 0; i < monkey_count; ++i) {
    if (i == m1) continue;  
    if (m2 == -1 || monkeys[i].inspect_count > monkeys[m2].inspect_count) 
      m2 = i;
  }


  return monkeys[m1].inspect_count * monkeys[m2].inspect_count;

}

static void print_monkeys(monkey_t* monkeys, int monkey_count) {
  for (int i = 0; i < monkey_count; ++i) {
    item_t *itr = monkeys[i].first;
    printf("Monkey #%d @ %3d: ", i, monkeys[i].inspect_count);
    while(itr != 0) {
      printf("%d ", itr->worry);
      itr = itr->next;
    }
    printf("\n");
  }

}

static int is_digit(char c) {
  return c >= '0' &&  
         c <= '9';
}

// only positive number. -1 is fail
static int parse_int(const char* buffer, int* at) {
  while(!is_digit(buffer[*at])) 
  {
    if (buffer[*at] == 0) 
    {
      return -1;
    }
    ++(*at);
  }

  int num = 0;
  while(is_digit(buffer[*at])) {
    num *= 10;
    num += buffer[*at] - '0'; 
    (*at)++;
  }
  return num;

}

int main() {
 
  FILE* fp = fopen("input.txt", "r");
  if (fp) {
    int item_count = 0;
    item_t* items = 0;

    int monkey_count = 0;
    monkey_t* monkeys = 0;

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
    printf("monkeys: %d\nitems: %d\n", monkey_count, item_count);
    monkeys = calloc(monkey_count, sizeof(monkey_t));
    items = calloc(item_count, sizeof(item_t));
    if (monkeys && items) 
    {
      fseek(fp, 0, SEEK_SET);

      int monkey_index = -1;
      int item_index = 0;
      while(fgets(buffer, array_count(buffer), fp)) {
        if (buffer[0] == 'M') ++monkey_index;
        else if (buffer[2] == 'S') {
          int at = 0;
          while(1) {
            int worry = parse_int(buffer, &at);
            if (worry == -1) break;
            item_t* item = items + item_index++;
            item->worry = worry;
            push_item(monkeys + monkey_index, item);
            //printf("%d ", worry);
          }
          //printf("\n");
        }
        else if (buffer[2] == 'O') 
        {
          if (buffer[23] == '+') {
            monkeys[monkey_index].op_type = OP_TYPE_ADD;  
            int at = 24;
            monkeys[monkey_index].op_val = parse_int(buffer, &at);
          }
          else if (buffer[23] == '*') {
            if (buffer[25] == 'o') {
              monkeys[monkey_index].op_type = OP_TYPE_SQ;  
            }
            else {
              int at = 24;
              monkeys[monkey_index].op_type = OP_TYPE_MUL;  
              monkeys[monkey_index].op_val = parse_int(buffer, &at);
            }
          }
        }
        else if (buffer[2] == 'T') {
          int at = 21;
          monkeys[monkey_index].test_val = parse_int(buffer, &at);
        }

        else if (buffer[7] == 't') {
          int at = 29; 
          monkeys[monkey_index].test_pass_monkey_index = parse_int(buffer, &at);
        }
        else if (buffer[7] == 'f') {
          int at = 30; 
          monkeys[monkey_index].test_fail_monkey_index = parse_int(buffer, &at);
        }
      }

#if 1
      // print monkey status
      for (int i = 0; i < monkey_count; ++i) {
        monkey_t* monkey = monkeys + i;
        printf("Monkey #%d\n", i);
        printf("  Starting items: ");
        for (item_t* it = monkey->first; 
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
      //print_monkeys(monkeys, monkey_count);
      for (int round = 1; round <= 20; ++round) 
      {
        for(int monkey_index = 0; 
            monkey_index < monkey_count; 
            ++monkey_index)
        {
          // inspect each item
          monkey_t* monkey = monkeys + monkey_index;
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

            int monkey_to_throw = 0;
            if ((monkey->first->worry % monkey->test_val) == 0) { 
              monkey_to_throw = monkey->test_pass_monkey_index;
            }
            else {
              monkey_to_throw = monkey->test_fail_monkey_index; 
            }

            printf("Monkey #%d: Throwing %d to %d\n", monkey_index, monkey->first->worry,  monkey_to_throw);

            item_t* item = pop_item(monkey);
            push_item(monkeys + monkey_to_throw, item);


            monkey->inspect_count++;
          }

#endif 
        }

        printf("=== Round %d ===\n", round);
        print_monkeys(monkeys, monkey_count);

      }

      printf("Business: %d\n", get_monkey_business(monkeys, monkey_count));





    }

    if (monkeys) free(monkeys);
    if (items)   free(items);

    fclose(fp);
  }
}

