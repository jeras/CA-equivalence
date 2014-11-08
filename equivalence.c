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
} t_ca;

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
//  0 |  0  0  0  0  1  1  1  1 
//  1 |  0  0  1  1  0  0  1  1 
//  2 |  0  1  0  1  0  1  0  1 
// ------------------------------
//    |  0  4  2  6  1  5  3  7 

// permute map
//
//  i |  0  1  2  3  4  5  6  7 
// ------------------------------
//  0 |  1  0  1  0  1  0  1  0 
//  1 |  1  1  0  0  1  1  0  0 
//  2 |  1  1  1  1  0  0  0  0 
// ------------------------------
//    |  7  6  5  4  3  2  1  0 

// mirror+permute map
//
//  i |  0  1  2  3  4  5  6  7 
// ------------------------------
//  0 |  1  1  1  1  0  0  0  0 
//  1 |  1  1  0  0  1  1  0  0 
//  2 |  1  0  1  0  1  0  1  0 
// ------------------------------
//    |  7  3  5  1  6  2  4  0 

int unsigned const map_mirror  [8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
int unsigned const map_permute [8] = { 7, 6, 5, 4, 3, 2, 1, 0 };

typedef struct {
  unsigned int cnt;
  unsigned int lnk;
} rule_element;

void rule2map (int unsigned rule, int unsigned *map, t_ca ca) {
  for (int unsigned i=0; i<ca.l; i++) {
    map[i] = rule % ca.k;
    rule   = rule / ca.k;
  }
}

void map2rule (int unsigned const *map, int unsigned *rule, t_ca ca) {
  *rule = 0;
  for (int unsigned i=0; i<ca.l; i++) {
    *rule = *rule * ca.k;
    *rule = *rule + map[ca.l-1-i];
  }
}

void print_map (int unsigned *map, t_ca ca) {
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
int unsigned transform_mirror (int unsigned rule, t_ca ca) {
  int unsigned mpi [ca.l];
  int unsigned mpo [ca.l];
  // convert rule to map
  rule2map (rule, mpi, ca);
  // mirror
  for (int unsigned i=0; i<ca.l; i++) {
    mpo [map_mirror [i]] = mpi[i];
  }
  // convert map to rule
  int unsigned ret;
  map2rule (mpo, &ret, ca);

  return ret;
}

// permute
int unsigned transform_permute (int unsigned rule, t_ca ca) {
  int unsigned mpi [ca.l];
  int unsigned mpo [ca.l];
  // convert rule to map
  rule2map (rule, mpi, ca);
  // permute
  for (int unsigned i=0; i<ca.l; i++) {
    mpo [map_permute [i]] = ! mpi[i];
  }
  // convert map to rule
  int unsigned ret;
  map2rule (mpo, &ret, ca);

  return ret;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main () {
  // CA lattice and neigborhood definitions
  t_ca ca;
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
    printf ("mirror  r=%03u, tmp=%03u\n", i, tmp);
    if (tmp<i) {
      while (rule_set[tmp].cnt == 0) {
        tmp = rule_set[tmp].lnk;
      }
      rule_set[tmp].cnt += rule_set[i].cnt;
      rule_set[i].cnt = 0;
      rule_set[i].lnk = tmp;
    } else {
    // permute
    tmp = transform_permute (i, ca);
    printf ("permute r=%03u, tmp=%03u\n", i, tmp);
    if (tmp<i) {
      while (rule_set[tmp].cnt == 0) {
        tmp = rule_set[tmp].lnk;
      }
      rule_set[tmp].cnt += rule_set[i].cnt;
      rule_set[i].cnt = 0;
      rule_set[i].lnk = tmp;
    }
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

  // print equivalence table
  typedef struct {
    unsigned int set [4];
    unsigned int cnt;
  } t_equ_tab;

  t_equ_tab equ_tab [len];

  for (int unsigned i=0; i<len; i++) {
    equ_tab[i].cnt = 0;
    tmp = rule_set[i].lnk;
    equ_tab[tmp].set[equ_tab[tmp].cnt] = i;
    equ_tab[tmp].cnt++;
  }

  for (int unsigned i=0; i<len; i++) {
    for (int unsigned j=0; j<equ_tab[i].cnt; j++) {
      printf (" %03u", equ_tab[i].set[j]);
    }
    printf ("\n");
  }

  int unsigned i;
  i = 25;
  printf ("r=%03u, mirror=%03u, permute=%03u\n", i, transform_mirror (i, ca), transform_permute (i, ca));
  i = 61;
  printf ("r=%03u, mirror=%03u, permute=%03u\n", i, transform_mirror (i, ca), transform_permute (i, ca));

  return (0);
}

