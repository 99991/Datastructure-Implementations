#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

hash_map *hash_map_create(int num_buckets, hash_fn_t fn) {
  hash_map *hm = (hash_map*)malloc(sizeof(hash_map));
  if (hm == NULL)
    return NULL;

  hm->buckets = (pair**)calloc(num_buckets, sizeof(pair*));
  if (hm->buckets == NULL) {
    free(hm);
    return NULL;
  }

  hm->num_buckets = num_buckets;
  hm->fn = fn;

  return hm;
}

void hash_map_destroy(hash_map *hm) {
  pair *p;

  for (int i = 0; i < hm->num_buckets; i++) {
    if ((p = (hm->buckets[i])) != NULL) {
      free(p);
    }
  }

  free(hm->buckets);
  free(hm);
}

void hash_map_set(hash_map *hm, void *key, void *value) {
  pair *p;
  int hash;

  p = (pair*)malloc(sizeof(pair));
  if (p == NULL) {
      fprintf(stderr, "[hash_map_set]: error allocating pair\n");
      return;
  }

  p->fst = key;
  p->snd = value;

  hash = hm->fn(key);

  hm->buckets[hash % hm->num_buckets] = p;
}

void *hash_map_get(hash_map *hm, void *key) {
  pair *p;
  int hash = hm->fn(key);

  p = hm->buckets[hash % hm->num_buckets];

  return p->snd;
}