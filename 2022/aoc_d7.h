
typedef enum {
  D7_NODE_TYPE_FILE,
  D7_NODE_TYPE_DIR,
} d7_node_type_t;

typedef struct d7_node_t {
  d7_node_type_t type;

  // we are either a file, which we only care about size, which is an s32_t
  // or we are a directory, which points to a node;
  union {
    u32_t size; 
    struct d7_node_t* head;
  };
  char name[128];
  struct d7_node_t* next;

  struct d7_node_t* parent;
} d7_node_t;

static u32_t
d7_node_size(d7_node_t* node) {
  //static s32_t scope = 0;
  d7_node_t* itr = node; 
  u32_t ret = 0;
  while(itr) {

    if (itr->type == D7_NODE_TYPE_FILE) {
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      //printf("file (%10s) (%d)\n", itr->name, itr->size);
      
      ret += itr->size;
    }
    else{
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      
      //scope+=2;
      //printf("dir  (%10s)\n", itr->name);
      u32_t sum = d7_node_size(itr->head);
      ret += sum;

      //scope-=2;
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      //printf("(%d)\n", sum);

    }
    itr = itr->next;
  }
  return ret;

}
#if 0
static d7_node_t make_file(const char* name, s32_t size){
  d7_node_t ret; 
  ret.d7_node_type = D7_NODE_TYPE_FILE;
  ret.name = name;
  ret.size = size;

  return ret;
}

static d7_node_t make_dir(const char* name) {
  d7_node_t ret;
  ret.d7_node_type = D7_NODE_TYPE_DIR;
  ret.name = name;
  ret.head = null;

  return ret;
}
#endif

typedef struct {
  u32_t start, ope;
} token_t;



static token_t 
next_token(tokenizer_t* t) {
  token_t ret = {0};

  // consume all white spaces
  while(is_whitespace(t->buffer[t->at])) 
    ++(t->at);

  if (t->buffer[t->at] == 0) {
    return ret;
  }

  ret.start = t->at; 

  while(!is_whitespace(t->buffer[t->at])) 
    ++(t->at);

  ret.ope = t->at;
  
  return ret;
}

static b32_t 
token_cmp(tokenizer_t tokenizer, token_t token, const char* str) {
  while(*str != 0) {
    if (*str != tokenizer.buffer[token.start]) { 
      return 0;
    }
    str++;
    token.start++;
  }
  return 1;
}


static u32_t
token_to_u32(tokenizer_t t, token_t token) {
  // very unsafe function, don't anyhow use! 
  u32_t num = 0;
  for(u32_t it = token.start; it < token.ope; ++it)
  {
    num *= 10;
    num += t.buffer[it] - '0'; 
  }
  return num;
}

#if 0
static void
d7_traverse(d7_node_t* node) {
  //static s32_t scope = 0;
  d7_node_t* itr = node; 
  while(itr) {
    if (itr->type == D7_NODE_TYPE_FILE) {
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      //printf("file (%10s) (%d)\n", itr->name, itr->size);
    }
    else{
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      //printf("dir  (%10s)\n", itr->name);
      
      //scope+=2;
      d7_traverse(itr->head);
      //scope-=2;

    }
    itr = itr->next;
  }

}
#endif

static u32_t
d7a_traverse_for_solution(d7_node_t* node, u32_t* dir_sizes) {
  //static s32_t scope = 0;
  d7_node_t* itr = node; 
  u32_t ret = 0;
  while(itr) {

    if (itr->type == D7_NODE_TYPE_FILE) {
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      //printf("file (%10s) (%d)\n", itr->name, itr->size);
      
      ret += itr->size;
    }
    else{
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      
      //scope+=2;
      //printf("dir  (%10s)\n", itr->name);
      u32_t sum = d7a_traverse_for_solution(itr->head, dir_sizes);
      ret += sum;

      if (sum <= 100000)
        *dir_sizes += sum;

      //scope-=2;
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      //printf("(%d) -> %d\n", sum, *dir_sizes);

    }
    itr = itr->next;
  }
  return ret;

}

static void
d7a(const char* filename) {
#define make_node() (nodes + node_count++);
#define push_node(head, node) (node)->next = (head); (head) = (node);
  d7_node_t nodes[512] = {0};
  u32_t node_count = 0;

  d7_node_t* cur_dir = make_node();
  cur_dir->type = D7_NODE_TYPE_DIR;
  cur_dir->name[0] = '/';

  FILE* fp = fopen(filename, "r");
  char buffer[128] = {0};
  if (fp) {

    while(fgets(buffer, sizeof(buffer), fp)) {

      ////////////////////////////////////////////
      // is a command
      if (buffer[0] == '$') 
      {
        //printf("command: ");
        tokenizer_t tokenizer = { buffer, 0 };

        // get rid of $
        next_token(&tokenizer);

        token_t command = next_token(&tokenizer);
        if (token_cmp(tokenizer, command, "cd")) {
          token_t dir_name =  next_token(&tokenizer);
          if (token_cmp( tokenizer, dir_name, "/")) {

            cur_dir = nodes + 0;
          }

          else if (token_cmp( tokenizer, dir_name, "..")) {
            if (cur_dir->parent) 
              cur_dir = cur_dir->parent;
          }
          else {
            // traverse the directory and set cur_dir to the node
            for(d7_node_t* itr = cur_dir->head; itr; itr = itr->next) 
            {
              if (itr->type == D7_NODE_TYPE_DIR) {
                if (token_cmp(tokenizer, dir_name, itr->name))
                {
                  // we found our dude
                  cur_dir = itr;
                  break;
                }


              }
            }

          }


        }
        else if (token_cmp(tokenizer, command, "ls"))
        {
          //printf( "ls");
        }
        else {
          //printf("DIE");
        }
          

        //printf("\n");

      }

      /////////////////////////////////////////////
      // Not a command. There is only two cases. Either:
      // - We are listing a file
      // - We are listing a directory
      else {
        tokenizer_t tokenizer = { buffer, 0 };
        token_t token = next_token(&tokenizer);
        if(is_digit(tokenizer.buffer[0])){
          // we are listing a file property here
          // currently the token should be containing a value
          d7_node_t* node = make_node();
          node->parent = cur_dir;
          node->size = token_to_u32(tokenizer, token);
          node->type = D7_NODE_TYPE_FILE;
          
          // we expect the 2nd token to be a name
          token_t name_token = next_token(&tokenizer);
          cstr_copy(node->name, tokenizer.buffer, name_token.start, name_token.ope);


          push_node(cur_dir->head, node);
          //printf("File found: (%s), (%d)\n", node->name, node->size);

        }

        else {
          // we are listing directory property here
          d7_node_t* node = make_node();
          node->type = D7_NODE_TYPE_DIR;
          node->parent = cur_dir;

          token_t name_token = next_token(&tokenizer);
          cstr_copy(node->name, tokenizer.buffer, name_token.start, name_token.ope);

          push_node(cur_dir->head, node);
          //printf("Dir found: (%s)\n", node->name);
        }

      }

    }
    fclose(fp);
  }


  //printf( "=== Checking contents ===\n");
  //d7_traverse(nodes + 0);
    

  //printf( "=== Checking contents ===\n");
  u32_t dir_sizes = 0;
  d7a_traverse_for_solution(nodes, &dir_sizes);
  printf("%d", dir_sizes);



}
static u32_t
d7b_traverse_for_solution(d7_node_t* node, u32_t size_to_cut, u32_t* size_found) {
  //static s32_t scope = 0;
  d7_node_t* itr = node; 
  u32_t ret = 0;
  while(itr) {

    if (itr->type == D7_NODE_TYPE_FILE) {
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      //printf("file (%10s) (%d)\n", itr->name, itr->size);
      
      ret += itr->size;
    }
    else{
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      
      //scope+=2;
      //printf("dir  (%10s)\n", itr->name);
      u32_t sum = d7b_traverse_for_solution(itr->head, size_to_cut, size_found);
      b32_t found = false;
      if (sum > size_to_cut) {
        if (sum < *size_found || *size_found == 0) {
          *size_found = sum;
          found = true;
        }
      }
      ret += sum;

      //scope-=2;
      //for(s32_t i = 0; i < scope; ++i) printf(" ");
      //printf("(%d) %s\n", sum, (found)?"!":"");

    }
    itr = itr->next;
  }
  return ret;

}

static void
d7b(const char* filename) {
#define make_node() (nodes + node_count++);
#define push_node(head, node) (node)->next = (head); (head) = (node);
  d7_node_t nodes[512] = {0};
  u32_t node_count = 0;

  d7_node_t* cur_dir = make_node();
  cur_dir->type = D7_NODE_TYPE_DIR;
  cur_dir->name[0] = '/';

  FILE* fp = fopen(filename, "r");
  char buffer[128] = {0};
  if (fp) {

    while(fgets(buffer, sizeof(buffer), fp)) {

      ////////////////////////////////////////////
      // is a command
      if (buffer[0] == '$') 
      {
        //printf("command: ");
        tokenizer_t tokenizer = { buffer, 0 };

        // get rid of $
        next_token(&tokenizer);

        token_t command = next_token(&tokenizer);
        if (token_cmp(tokenizer, command, "cd")) {
          token_t dir_name =  next_token(&tokenizer);
          if (token_cmp( tokenizer, dir_name, "/")) {

            cur_dir = nodes + 0;
          }

          else if (token_cmp( tokenizer, dir_name, "..")) {
            if (cur_dir->parent) 
              cur_dir = cur_dir->parent;
          }
          else {
            // traverse the directory and set cur_dir to the node
            for(d7_node_t* itr = cur_dir->head; itr; itr = itr->next) 
            {
              if (itr->type == D7_NODE_TYPE_DIR) {
                if (token_cmp(tokenizer, dir_name, itr->name))
                {
                  // we found our dude
                  cur_dir = itr;
                  break;
                }


              }
            }

          }


        }
        else if (token_cmp(tokenizer, command, "ls"))
        {
          //printf( "ls");
        }
        else {
          //printf("DIE");
        }
          

        //printf("\n");

      }

      /////////////////////////////////////////////
      // Not a command. There is only two cases. Either:
      // - We are listing a file
      // - We are listing a directory
      else {
        tokenizer_t tokenizer = { buffer, 0 };
        token_t token = next_token(&tokenizer);
        if(is_digit(tokenizer.buffer[0])){
          // we are listing a file property here
          // currently the token should be containing a value
          d7_node_t* node = make_node();
          node->parent = cur_dir;
          node->size = token_to_u32(tokenizer, token);
          node->type = D7_NODE_TYPE_FILE;
          
          // we expect the 2nd token to be a name
          token_t name_token = next_token(&tokenizer);
          cstr_copy(node->name, tokenizer.buffer, name_token.start, name_token.ope);


          push_node(cur_dir->head, node);
          //printf("File found: (%s), (%d)\n", node->name, node->size);

        }

        else {
          // we are listing directory property here
          d7_node_t* node = make_node();
          node->type = D7_NODE_TYPE_DIR;
          node->parent = cur_dir;

          token_t name_token = next_token(&tokenizer);
          cstr_copy(node->name, tokenizer.buffer, name_token.start, name_token.ope);

          push_node(cur_dir->head, node);
          //printf("Dir found: (%s)\n", node->name);
        }

      }

    }
    fclose(fp);
  }


  //printf( "=== Checking contents ===\n");
  //d7_traverse(nodes + 0);
    

  //printf( "=== Checking sizes ===\n");
  u32_t total_size = d7_node_size(nodes);
  //printf("total_size: %d\n", total_size);
  u32_t size_to_cut = total_size - 40000000;
  //printf("size_to_cut: %d\n", size_to_cut);

  //printf( "=== Checking contents ===\n");
  u32_t size_found = 0;
  d7b_traverse_for_solution(nodes, size_to_cut, &size_found );
  printf("%d", size_found);

}
