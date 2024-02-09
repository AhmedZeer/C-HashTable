#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"
#define HT_INITIAL_BASE_SIZE

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


static int ht_hash( const char* s, const int a, const int m){
  int i;
  long hash = 0;
  const int lens_s = strlen(s);
  for ( i = 0 ; i < lens_s ; i++) {
    hash += (long)pow(a, lens_s - (i+1)) * s[i];
  }
  return (int)hash /= m;
}

static int ht_get_hash( const char* s , const int num_buckets , const int attempt){
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets );
  const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets );
  return ( hash_a + (attempt * (hash_b + 1))) % num_buckets;
}


void ht_insert(ht_hash_table* ht, const char* key, const char* value){

  const int load = (ht->size / ht->count) * 100;
  if( load >= 70 ){
    ht_resize_up(ht);
  }

  ht_item* item = ht_new_item(key,value);
  int index = ht_get_hash(item->key, ht->size, 0);
  ht_item* curr_item = ht->item[index];
  int i = 0;
  while(curr_item){
    if( curr_item != &HT_DELETED_ITEM ){
      if(strcmp(key, item->key) == 0){
        ht_del_item(curr_item);
        ht->items[index] = item;
        return;
      }
    }
    index = ht_get_hash(item->key, ht->size, i);
    curr_item = ht->item[index];
    i++;
  }
  ht->item[index] = item;
  ht->count++;
}

char* ht_search(ht_hash_table* ht, char* key){
  int index = ht_get_hash(key, ht->count , 0);
  ht_item* item = ht->items[index];
  int i = 1;
  while(!item){
    if( item != &HT_DELETED_ITEM ){
      if( strcmp(key, item->key) == 0 ){
        return item->value;
      }
      index = ht_get_hash(key, ht->count, i);
      item = ht->items[index];
      i++;
    }
  }
  return NULL;
}


void ht_delete(ht_hash_table* ht, char* key){
  const int load = ( ht->count / ht->size )* 100;
  if(load < 10){
    ht_resize_down(ht);
  }
  int index = ht_get_hash(key, ht->count, 0);
  ht_item* item =  ht->items[index];
  char status = 0;
  while(curr_item && !status){
    if(item != &HT_DELETED_ITEM){
      if( strcmp(item->key,key) == 0 ){
        ht->items[index] = &HT_DELETED_ITEM;
        status = 1;
      }
    }
    index = ht_get_hash(key, ht->count, i);
    item = ht->items[index];
    i++;
  }
  if(status){
    ht->count--;
  }
}

static ht_hash_table* ht_new_sized( const int base_size ){
  ht_hash_table* ht = xmalloc(sizeof(ht_hash_table));
  ht->base_size = base_size;
  ht->size = next_prime(ht->base_size);
  ht->count = 0;
  ht->items = xcalloc((size_t)ht->size, sizeof(ht_items*));
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
  new_ht->size = tmp_size->size;

  ht_item** tmp_items = ht->items;
  ht->items = new_ht->items;
  new_ht->items = tmp_items;

  ht_del_hash_table(new_ht);

}

static void ht_resize_up(ht_hash_table* ht){
  const int new_size = ht->base_size * 2;
  ht_resize(ht, new_size);
}

static void ht_resize_down(ht_hash_table* ht){
  const int new_size = ht->base_size / 2;
  ht_resize(ht, new_size);
}



int main(int argc, char *argv[])
{
  ht_hash_table* ht = ht_new();
  ht_del_hash_table(ht);
  printf("ahello\n");
  return EXIT_SUCCESS;
}
