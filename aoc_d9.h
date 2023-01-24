#define abs_of(x) ((x) < 0) ? -(x) : (x)

typedef struct {
  u32_t x;
  u32_t y;
} v2u_t;

typedef struct {
  v2u_t* e;
  u32_t count;
  u32_t cap;
} d9_list_t;

static int 
d9_list_exists(d9_list_t* list, v2u_t pos) {
  for (int i = 0; i < list->count; ++i) {
    if (list->e[i].x == pos.x && list->e[i].y == pos.y) {
      return 1;
    }
  }
  return 0;
}


static int 
d9_list_insert_unique(d9_list_t* list, v2u_t pos) {
  if (!d9_list_exists(list, pos)) {
    if (list->count >= list->cap) {

      list->cap *= 2;

      void* mem = realloc(list->e, sizeof(v2u_t)*list->cap);
      if (!mem) {
        printf("Realloc failed\n");
        return 0;
      }
      list->e = mem; 

    }
    list->e[list->count++] = pos;
  }

  return 1;
}

static int 
d9_list_init(d9_list_t* list) {
  list->cap = 1024;
  list->count = 0;
  list->e = malloc(sizeof(v2u_t)*1024);

  if (!list->e) return 0;
  return 1;
}

static void 
d9_list_free(d9_list_t* list) {
  free(list->e);
}

static int 
move(v2u_t* head, v2u_t* tail, d9_list_t* visited, char direction) {
  v2u_t old_head = *head;
  switch(direction) {
    case 'U': {
      head->y += 1;
    } break;
    case 'D': {
      head->y -= 1;
    } break;
    case 'L': {
      head->x -= 1;
    } break;
    case 'R': {
      head->x += 1;
    } break;
  }

  // resolve tail
  // is head too far from tail
  s32_t disp_x = head->x - tail->x;
  s32_t disp_y = head->y - tail->y;

  int dist_x = abs_of(disp_x);
  int dist_y = abs_of(disp_y);

  // NOTE(momo): we don't have to move diagonally.
  // We just have to move to where head once was! 
  if (dist_x + dist_y > 2) {
    *tail = old_head;
  }
  else {
    if (disp_x > 1) {
      tail->x += 1;
    }
    else if(disp_x < -1) {
      tail->x -= 1;
    }

    if (disp_y > 1) {
      tail->y += 1;
    }
    else if(disp_y < -1) {
      tail->y -= 1;
    }
  }

#if 0
  printf("Move %c\n", direction);
  printf("head: %d %d\n", head->x, head->y);
  printf("tail: %d %d\n\n", tail->x, tail->y);
#endif

  if (!d9_list_insert_unique(visited, *tail)) {
    return 0;
  }
  

  return 1;
}



static void
d9a(const char* filename) {
  FILE* fp = fopen(filename, "r");
  d9_list_t visited = {0};
 
  if (fp) 
  {
    if (d9_list_init(&visited)) 
    {
      v2u_t head = {0};
      v2u_t tail = {0};

      d9_list_insert_unique(&visited, tail);

      char buffer[10];
      while (fgets(buffer, sizeof(buffer), fp))
      {
        char direction = buffer[0];
        int amount = cstr_to_u32(buffer + 2); // super hacky haha 
        for(int i = 0; i < amount; ++i) {
          if (!move(&head, &tail, &visited, direction)) {
            goto die;
          }
        }
        

      }

      printf("%d\n", visited.count);
      
    }
    else {
      printf("Fail to init list\n");
    }
die:
    fclose(fp);
  }

  d9_list_free(&visited);

}
