////////////////////////////////////////////////////////////////////////////////
// brute-force tool for searching CA rule equivalence groups
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// CA definition structure
////////////////////////////////////////////////////////////////////////////////

typedef struct {
  unsigned int k;
  unsigned int n;
  unsigned int l;
} t_rule;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int unsigned ipow(int unsigned base, int unsigned exp) {
  int unsigned result = 1;
  while (exp) {
    if (exp & 1)
      result *= base;
    exp >>= 1;
    base *= base;
  }
  return result;
}

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

void rule2map (int unsigned rule, int unsigned *map, t_rule ca) {
  for (int unsigned i=0; i<ca.l; i++) {
    map[i] = rule % ca.k;
    rule   = rule / ca.k;
  }
}

void map2rule (int unsigned const *map, int unsigned *rule, t_rule ca) {
  *rule = 0;
  for (int unsigned i=0; i<ca.l; i++) {
    *rule = *rule * ca.k;
    *rule = *rule + map[i];
  }
}

void print_map (int unsigned *map, t_rule ca) {
  printf ("map = ");
  for (int unsigned i=0; i<ca.l; i++) {
    printf ("%u ", map[i]);
  }
  printf ("\n");
};

////////////////////////////////////////////////////////////////////////////////
// transformations keeping equivalence
////////////////////////////////////////////////////////////////////////////////

// mirror
int unsigned transform_mirror (int unsigned rule, t_rule ca) {
  int unsigned mpi [ca.l];
  int unsigned mpo [ca.l];
  // convert rule to map
  rule2map (rule, mpi, ca);
  // mirror
  for (int unsigned i=0; i<ca.l; i++) {
    mpo [i] = mpi[map_mirror [i]];
  }
  // convert map to rule
  map2rule (mpo, &rule, ca);

  return rule;
}

// permute
int unsigned transform_permute (int unsigned rule, t_rule ca) {
  int unsigned map [ca.l];
  // convert rule to map
  rule2map (rule, map, ca);
  // permute
  for (int unsigned i=0; i<ca.l; i++) {
    map [i] = ! map [i];
  }
  // convert map to rule
  map2rule (map, &rule, ca);

  return rule;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main () {
  // CA lattice and neigborhood definitions
  t_rule ca;
  ca.k = 2;
  ca.n = 3;
  ca.l = ipow (ca.k, ca.n);
  int unsigned len = ipow (ca.k, ipow (ca.k, ca.n));
  printf ("There are %u rules.\n", len);
  rule_element rule_set [len];
  int unsigned tmp;

  for (int unsigned i=0; i<len; i++) {
    // initialize the counter to 1 and link to self
    rule_set[i].cnt = 1;
    rule_set[i].lnk = i;
    // mirror
    tmp = transform_mirror (i, ca);
    if (tmp<i) {
      rule_set[tmp].cnt += rule_set[i].cnt;
      rule_set[i].cnt = 0;
      rule_set[i].lnk = tmp;
    }
    // permute
    tmp = transform_permute (i, ca);
    printf ("r=%03u, tmp=%03u\n", i, tmp);
    if (tmp<i) {
      rule_set[tmp].cnt += rule_set[i].cnt;
      rule_set[i].cnt = 0;
      rule_set[i].lnk = tmp;
    }
  }

  // print equivalence table
  int unsigned sum_cnt = 0;
  int unsigned equ_cnt = 0;
  for (int unsigned i=0; i<len; i++) {
    printf ("r=%03u, lnk=%03u, cnt=%03u\n", i, rule_set[i].lnk, rule_set[i].cnt);
    sum_cnt += rule_set[i].cnt;
    equ_cnt += rule_set[i].cnt > 0;
  }
  printf ("sum_cnt=%u\n", sum_cnt);
  printf ("equ_cnt=%u\n", equ_cnt);

  return (0);
}

