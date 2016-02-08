#!/usr/bin/env python

# CA properties
k=2 # number of states per cell
n=3 # number of cells in a neighborhood

# number of all different neighborhoods
nbh_all = k**n

# number of all different rules 
N_all = k**nbh_all

# number of neighborhoods which are mirror images of themselves
if (n%2): nbh_sym_mir = k**(n/2+1)  # odd  neighborhood size
else:     nbh_sym_mir = k**(n/2)    # even neighborhood size

# number of neighborhoods where permutation and mirroring (both applied) map into the same neihgborhood
if (n%2): nbh_sym_mir_per = 0          # odd  neighborhood size
else:     nbh_sym_mir_per = k**(n/2)   # even neighborhood size

# number of rules which are symmetric to mirroring
# There are (nbh_all-nbh_sym_mir) neighborhoods which do not mirror into themselves, for rules to be
# mirror symmetric they and their mirror image must map into the same state. Since half of this neighborhoods are
# the mirror image of the other half, there are k**((nbh_all-nbh_sym_mir)/2) such mappings.
# For each such mapping, any mapping of the remaining symmetric neighborhoods is allowed
# N_sym_mir = k**nbh_sym_mir * k**((nbh_all-nbh_sym_mir)/2)
# N_sym_mir = k**(nbh_sym_mir+(nbh_all-nbh_sym_mir)/2)
N_sym_mir = k**((nbh_all+nbh_sym_mir)/2)

# number of rules which are symmetric to permutation
# 
N_sym_per = k**(nbh_all/k) # TODO

# number of rules which exibit both mirror and permutation symetry
N_sym_mir_per_either = k**((nbh_sym_mir-nbh_sym_mir_per)/k/2) * k**((nbh_sym_mir_per)/k) * k**((nbh_all-nbh_sym_mir)/2)
#N_sym_mir_per_either = 8 # TODO

# number of rules which are only symmetric after both transformations are applied
N_sym_mir_per_both = 8 # TODO

# sum of all rule groups
N_all_calc = N_asm + (N_sym_mir - N_sym_mir_per_either) + (N_sym_per - N_sym_mir_per_either) + N_sym_mir_per_either + N_sym_mir_per_both

# assymetric rules
N_asm = N_all - (N_sym_mir - N_sym_mir_per_either) - (N_sym_per - N_sym_mir_per_either) - N_sym_mir_per_either - N_sym_mir_per_both

# number of equivalence classes
E = N_asm/4 + (N_sym_mir - N_sym_mir_per_either)/2 + (N_sym_per - N_sym_mir_per_either)/2 + N_sym_mir_per_either + N_sym_mir_per_both/2


print "nbh_all            = " + str(nbh_all           )
print "N_all              = " + str(N_all             )
print "nbh_sym_mir        = " + str(nbh_sym_mir       )
print "N_sym_mir          = " + str(N_sym_mir         )
print "N_sym_per          = " + str(N_sym_per         )
print "N_asm              = " + str(N_asm             )
print "N_all_calc         = " + str(N_all_calc        )
print "E                  = " + str(E                 )


## list of rules symmetric to either permutation or mirroring
#bin( 23) = '0b 00010111'
#bin( 51) = '0b 00110011'
#bin( 77) = '0b 01001101'
#bin(105) = '0b 01101001'
#bin(232) = '0b 11101000'
#bin(204) = '0b 11001100'
#bin(178) = '0b 10110010'
#bin(150) = '0b 10010110'
