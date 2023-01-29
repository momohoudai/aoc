
typedef struct {
  v2s_t* e;
  u32_t count;
  u32_t cap;
} d9_list_t;

static int 
d9_list_exists(d9_list_t* list, v2s_t pos) {
  for (int i = 0; i < list->count; ++i) {
    if (list->e[i].x == pos.x && list->e[i].y == pos.y) {
      return 1;
    }
  }
  return 0;
}


static b32_t 
d9_list_insert_unique(d9_list_t* list, v2s_t pos) {
  if (!d9_list_exists(list, pos)) {
    if (list->count >= list->cap) {

      list->cap *= 2;

      void* mem = realloc(list->e, sizeof(v2s_t)*list->cap);
      if (!mem) {
        printf("Realloc failed\n");
        return false;
      }
      list->e = mem; 

    }
    list->e[list->count++] = pos;
  }

  return true;
}

static int 
d9_list_init(d9_list_t* list) {
  list->cap = 1024;
  list->count = 0;
  list->e = malloc(sizeof(v2s_t)*1024);

  if (!list->e) return 0;
  return 1;
}

static void 
d9_list_free(d9_list_t* list) {
  free(list->e);
}

static b32_t 
d9_move(v2s_t* head, v2s_t* tail, d9_list_t* visited, char direction) 
{
  v2s_t old_head = *head;
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

  s32_t dist_x = abs_of(disp_x);
  s32_t dist_y = abs_of(disp_y);

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
    return false;
  }
  

  return true;
}



static void
d9a(const char* filename) {
  FILE* fp = fopen(filename, "r");
  d9_list_t visited = {0};
 
  if (fp) 
  {
    if (d9_list_init(&visited)) 
    {
      v2s_t head = {0};
      v2s_t tail = {0};

      d9_list_insert_unique(&visited, tail);

      char buffer[10];
      while (fgets(buffer, sizeof(buffer), fp))
      {
        char direction = buffer[0];
        int amount = cstr_to_u32(buffer + 2); // super hacky haha 
        for(int i = 0; i < amount; ++i) {
          if (!d9_move(&head, &tail, &visited, direction)) {
            goto die;
          }
        }
        

      }

      printf("%d", visited.count);
      
    }
    else {
      printf("Fail to init list\n");
    }
die:
    fclose(fp);
  }

  d9_list_free(&visited);

}

static int 
d9_follow(v2s_t* head, v2s_t* tail) {
  // resolve tail
  // is head too far from tail
  s32_t disp_x = head->x - tail->x;
  s32_t disp_y = head->y - tail->y;

  s32_t dist_x = abs_of(disp_x);
  s32_t dist_y = abs_of(disp_y);

  if (dist_x + dist_y >= 3) {
    // diagonals case
    if (disp_x >= 2){
      tail->x += 1;
      if (disp_y < 0)
        tail->y -= 1;
      else 
        tail->y += 1;
    }
    else if (disp_y >= 2){
      tail->y += 1;
      if (disp_x < 0)
        tail->x -= 1;
      else 
        tail->x += 1;
    }
    else if (disp_x <= -2)
    {
      tail->x -= 1;
      if (disp_y < 0) 
        tail->y -=1;
      else
        tail->y +=1;
    }
    else if (disp_y <= -2)
    {
      tail->y -= 1;
      if (disp_x < 0) 
        tail->x -=1;
      else
        tail->x +=1;
    }
  }
  else {
    if (disp_x >= 2) {
      tail->x += 1;
    }
    else if(disp_x <= -2) {
      tail->x -= 1;
    }

    if (disp_y >= 2) {
      tail->y += 1;
    }
    else if(disp_y <= -2) {
      tail->y -= 1;
    }
  }

#if 0 
  printf("Move %c\n", direction);
  printf("head: %d %d\n", head->x, head->y);
  printf("tail: %d %d\n\n", tail->x, tail->y);
#endif

  return 1;
}


static void
d9b(const char* filename) {
  FILE* fp = fopen(filename, "r");
  d9_list_t visited = {0};
 
  if (fp) 
  {
    if (d9_list_init(&visited)) 
    {
      v2s_t nodes[10] = {0};

      d9_list_insert_unique(&visited, nodes[9]);

      char buffer[10];
      while (fgets(buffer, sizeof(buffer), fp))
      {
        char direction = buffer[0];
        
        u32_t amount = cstr_to_u32(buffer + 2); // super hacky haha 
                                             //
        //printf("%c %d\n", direction, amount);
        for(u32_t i = 0; i < amount; ++i) {

          switch(direction) {
            case 'U': {
              nodes[0].y += 1;
            } break;
            case 'D': {
              nodes[0].y -= 1;
            } break;
            case 'L': {
              nodes[0].x -= 1;
            } break;
            case 'R': {
              nodes[0].x += 1;
            } break;
          }

          for (u32_t node_index = 0; node_index < array_count(nodes)-1; ++node_index) {
            v2s_t* head = nodes + node_index;
            v2s_t* tail = nodes + node_index + 1;

            d9_follow(head,  tail);
          }

#if 0
          for (int node_index = 0; node_index < arr_count(nodes); ++node_index) {
            printf("#%d: %d %d\n", node_index, nodes[node_index].x, nodes[node_index].y);
          }
          printf("\n");
#endif
          if (!d9_list_insert_unique(&visited, nodes[9])) {
            //printf("Died 2\n");
            goto die;
          }
        }
      }

#if 0
      for(int i = 0; i < visited.count; ++i) {
        printf("%d %d\n", visited.e[i].x, visited.e[i].y);
      }
#endif

      printf("%d", visited.count);
      
    }
    else {
      printf("Fail to init list\n");
    }
die:
    fclose(fp);
  }

  d9_list_free(&visited);

}
