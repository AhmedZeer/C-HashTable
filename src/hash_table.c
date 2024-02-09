#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL};

//Creates an item.
static ht_item* ht_new_item( const char* k, const char* v){
  ht_item* i = malloc(sizeof(ht_item));
  i->key = strdup(k);
  i->value = strdup(v);
  return i;
}

//Crates a Hash Table.
ht_hash_table* ht_new(){
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));
  ht->size = 53;
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
  return ht;
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
  ht_item* item = ht_new_item(key,value);
  int index = ht_get_hash(item->key, ht->size, 0);
  ht_item* curr_item = ht->item[index];
  int i = 0;
  while(curr_item){
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
    if( strcmp(key, item->key) == 0 ){
      return item->value;
    }
    index = ht_get_hash(key, ht->count, i);
    item = ht->items[index];
    i++;
  }
  return NULL;
}


void ht_delete(ht_hash_table* ht, char* key){
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

int main(int argc, char *argv[])
{
  ht_hash_table* ht = ht_new();
  ht_del_hash_table(ht);
  printf("ahello\n");
  return EXIT_SUCCESS;
}


