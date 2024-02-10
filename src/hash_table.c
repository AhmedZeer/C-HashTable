#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "hash_table.h"
#define HT_INITIAL_BASE_SIZE 50
#define HT_PRIME_2 61
#define HT_PRIME_1 71


static ht_item HT_DELETED_ITEM = {NULL, NULL};

//Creates an item.
static ht_item* ht_new_item( const char* k, const char* v){
  ht_item* i = malloc(sizeof(ht_item));
  i->key = strdup(k);
  i->value = strdup(v);
  return i;
}


//Free an item.
static void ht_del_item(ht_item* i){
  free(i->key);
  free(i->value);
  free(i);
}


//Delete the whole table.
void ht_del_hash_table(ht_hash_table* ht){
  int i;
  for ( i = 0 ; i < ht->size ; i++ ) {
    ht_item* item = ht->items[i];
    if(item != NULL){
      ht_del_item(item);
    }
  }
  free(ht->items);
  free(ht);
}

static void ht_resize_up(ht_hash_table* ht){
  const int new_size = ht->base_size * 2;
  ht_resize_up(ht);
}

static void ht_resize_down(ht_hash_table* ht){
  const int new_size = ht->base_size / 2;
  ht_resize_down(ht);
}


static int ht_hash( const char* s, const int a, const int m){
  int i;
  long hash = 0;
  const int lens_s = strlen(s);
  for ( i = 0 ; i < lens_s ; i++) {
    hash += (long)pow(a, lens_s - (i+1)) * s[i];
  }
  hash /= m;
  return (int)hash; /*1st Error */
}

static int ht_get_hash( const char* s , const int num_buckets , const int attempt){
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets );
  const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets );
  printf("final HASH: %d\n", ( hash_a + (attempt * (hash_b + 1))) % num_buckets );
  return ( hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

char* ht_search( ht_hash_table* ht, const char* key ){
  int index = ht_get_hash(key, ht->size , 0);
  printf("the index search: %d\n", index);
  ht_item* item = ht->items[index];
  int i = 1;
  while(item){
    if( item != &HT_DELETED_ITEM ){
      if( strcmp(key, item->key) == 0 ){
        printf("found in func.\n");
        char* retrn = item->value;
        return retrn;
      }
      index = ht_get_hash(key, ht->size, i);
      item = ht->items[index];
      i++;
    }
  }
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value){

  /*
  const int load = (ht->size / ht->count) * 100;
  if( load >= 70 ){
    ht_resize_up(ht);
  }
  */

  ht_item* item = ht_new_item(key,value);
  int index = ht_get_hash(item->key, ht->size, 0);
  ht_item* curr_item = ht->items[index];
  printf("the index in insert func: %d\n", index);
  int i = 0;
  while(curr_item){
    if( curr_item != &HT_DELETED_ITEM ){
      if(strcmp(key, item->key) == 0){
        ht_del_item(curr_item);
        ht->items[index] = item;
        printf("inserted!\n");
        printf("final index is %d:\n", index);
        return;
      }
    }
    index = ht_get_hash(item->key, ht->size, i);
    curr_item = ht->items[index];
    i++;
  }
  ht->items[index] = item;
  printf("final index is %d:\n", index);
  printf("inserted!\n");
  ht->count++;
}



void ht_delete(ht_hash_table* ht, const char* key){
  const int load = ( ht->count / ht->size )* 100;
  if(load < 10){
    ht_resize_down(ht);
  }
  int index = ht_get_hash(key, ht->count, 0);
  ht_item* item =  ht->items[index];
  char status = 0;
  while(item && !status){
    if(item != &HT_DELETED_ITEM){
      if( strcmp(item->key,key) == 0 ){
        ht->items[index] = &HT_DELETED_ITEM;
        status = 1;
      }
    }
    int i;
    index = ht_get_hash(key, ht->count, i);
    item = ht->items[index];
    i++;
  }
  if(status){
    ht->count--;
  }
  return;
}

static ht_hash_table* ht_new_sized( const int base_size ){
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));
  ht->base_size = base_size;
  ht->size = next_prime(ht->base_size);
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
  return ht;
}

ht_hash_table* ht_new(){
  return ht_new_sized(HT_INITIAL_BASE_SIZE);

}

static void ht_resize(ht_hash_table* ht, int base_size){
  if( base_size < HT_INITIAL_BASE_SIZE ){
    return;
  }
  ht_hash_table* new_ht = ht_new_sized(base_size);
  int i;
  for( i = 0 ; i < new_ht->size ; i++){
    ht_item* item = ht->items[i];
    if( item && item != &HT_DELETED_ITEM ){
      ht_insert(new_ht, item->key, item->value);
    }
  }

  ht->base_size = new_ht->base_size;
  ht->count = new_ht->count;

  const int tmp_size = ht->size;
  ht->size = new_ht->size;
  new_ht->size = tmp_size;

  ht_item** tmp_items = ht->items;
  ht->items = new_ht->items;
  new_ht->items = tmp_items;

  ht_del_hash_table(new_ht);

}



int main(int argc, char *argv[])
{
  printf("here we start\n");

  const char* key = "100";
  const char* value = "1stElm"; 

  ht_hash_table* ht = ht_new();
  printf("here we intited the table.\n");

  ht_insert(ht,key,value);

  printf("value of search: %s\n", ht_search(ht,key));

  /*
  printf("table test: %s\n",ht->items[8]->key);
  printf("table test: %s\n",ht->items[8]->value);
  char* search = ht_search(ht, key);
  printf("search value: %s\n",search);
  printf("here we searched.\n");
  */

  

  printf("here it ends.\n");

  return EXIT_SUCCESS;
}
