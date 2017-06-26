#!/usr/bin/python3

import sys
import random

if (len(sys.argv) < 2):
    print('Usage: python matrix-gen.py N')
    sys.exit()

n = int(sys.argv[1])

max_cost = 100 # Maximum cost between cities

print('Generating matrix for {} cities.'.format(n))
f = open('{}.mtx'.format(n), 'w')
f.write('{}\n'.format(n))

for y in range(n):
  for x in range(n - y - 1):
    f.write('{} '.format(random.randint(1, max_cost)))
  f.write('\n')

f.close()

