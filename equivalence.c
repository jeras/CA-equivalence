#include <math.h>

// original map
//
//  i |  0  1  2  3  4  5  6  7 
// ------------------------------
//  0 |  0  1  0  1  0  1  0  1 
//  1 |  0  0  1  1  0  0  1  1 
//  2 |  0  0  0  0  1  1  1  1 

// mirror map
//
//  i |  0  1  2  3  4  5  6  7 
// ------------------------------
//  2 |  0  0  0  0  1  1  1  1 
//  1 |  0  0  1  1  0  0  1  1 
//  0 |  0  1  0  1  0  1  0  1 

int unsigned const map_mirror [8] = { 0, 4, 2, 6, 1, 5, 3, 7 };


typedef struct {
  unsigned int cnt;
  unsigned int lnk;
} rule_element;



void rule2map (int unsigned rule, int unsigned *map, int unsigned k, int unsigned n) {
  for (int i; i<n; i++) {
    map[i] = rule % k;
    rule   = rule / k;
  }
}

void map2rule (int unsigned const *map, int unsigned *rule, int unsigned k, int unsigned n) {
  for (int i; i<n; i++) {
    *rule = *rule * k;
    *rule = *rule + map[i];
  }
}

// mirror
int unsigned transform_mirror (int unsigned rule, int unsigned k, int unsigned n) {
  int len = pow (k, n);
  int unsigned mpi [len];
  int unsigned mpo [len];
  // convert rule to map
  rule2map (rule, mpi, k, n);
  // mirror
  for (int i=0; i<len; i++) {
    mpo [i] = mpi[map_mirror [i]];
  }
  // convert map to rule
  map2rule (mpo, &rule, k, n);

  return rule;
}

// permute
int unsigned transform_permute (int unsigned rule, int unsigned k, int unsigned n) {
  int len = pow (k, n);
  int unsigned map [len];
  // convert rule to map
  rule2map (rule, map, k, n);
  // permute
  for (int i=0; i<len; i++) {
    map [i] = ! map [i];
  }
  // convert map to rule
  map2rule (map, &rule, k, n);

  return rule;
}


int main () {
  int unsigned k = 2;
  int unsigned n = 3;
  int len = pow (k, pow (k, n));
  rule_element rule_set [len];
  int unsigned tmp;

  for (int i=0; i<len; i++) {
    // initialize the counter
    rule_set[i].cnt = 1;
    // mirror
//    while 
    tmp = transform_mirror (i, k, n);
    if (tmp<i) {
      rule_set[i].cnt = 0;
      rule_set[i].lnk = tmp;
      rule_set[tmp].cnt++;
    }
    // permute
  }
}

