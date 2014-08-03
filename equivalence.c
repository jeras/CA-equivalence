struct {
  unsigned int cnt;
  unsigned int lnk;
} rule;



void rule2map (int unsigned val, int unsigned *map, int unsigned k, int unsigned n) {
  for (int i; i<n; i++) {
    map[i] = val % k;
    val    = val / k;
  }
}

void map2rule (int unsigned map, int unsigned *val, int unsigned k, int unsigned n) {
  for (int i; i<n; i++) {
    val = val * k;
    val = val + map[i];
  }
}

// mirror
int unsigned transform_mirror (int unsigned val) {
  int len = k ** n;
  int unsigned map [len];
  // convert rule to map
  rule2map (val, map, k, n);
  // mirror
  for (int i=0; i<len; i++) {
    map [i] = ! map [i];
  }
  // convert map to rule
  map2rule (val, map, k, n);

  return val;
}

// permute
int unsigned transform_permute (int unsigned val) {
  int len = k ** n;
  int unsigned map [len];
  // convert rule to map
  rule2map (val, map, k, n);
  // permute
  for (int i=0; i<len; i++) {
    map [i] = ! map [i];
  }
  // convert map to rule
  map2rule (val, map, k, n);

  return val;
}


int main {
  len = k ** (k ** n);
  rule ruleset [len];
  int unsigned tmp;

  for (int i=0; i<len; i++) {
    // initialize the counter
    ruleset[i].cnt = 1;
    // mirror
    while 
    tmp = transform_mirror[i];
    if (tmp<i) {
      ruleset[i].cnt = 0;
      ruleset[i].lnk = tmp;
      ruleset[tmp].cnt++;
    }
    // permute
  }
}

