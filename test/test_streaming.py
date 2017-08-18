#!/usr/bin/env python

from __future__ import print_function

import os
import random
import resource
import sys

from rabin import Rabin, get_file_fingerprints, set_min_block_size, set_max_block_size, set_average_block_size

if len(sys.argv) == 3:
    stream_count,seed = map(int, sys.argv[1:]) # 100
    seed = int(sys.argv[2]) # $RANDOM
else:
    stream_count = 10
    seed = random.random()
print('seed', seed)

stream_bs = 1024 * 1024 
stream_len = stream_bs * stream_count
max_mem = 1024 * 1024 * 20

resource.setrlimit(resource.RLIMIT_AS, (max_mem,-1))
# print(resource.getrlimit(resource.RLIMIT_AS))

random.seed(seed)

max_blocksize = random.randint(512, max_mem/10)

set_min_block_size(int(max_blocksize/10))
set_max_block_size(max_blocksize)
set_average_block_size(int(max_blocksize/5))

reached = []
def block_reached(start, length, fingerprint):
    # print('(%s, %s, %s)' % (start, length, fingerprint))
    reached.append((start, length, fingerprint))


r = Rabin()
r.register(block_reached)

try:
    from guppy import hpy; hp=hpy()
except:
    pass

total = 0
while total < stream_len:
    size = random.randint(1,max_blocksize*2)
    size = min(size, stream_len - total)
    # print(size),
    data = ''
    for i in range(size):
        data += chr(random.randrange(0,256))
    r.update(data)
    total += len(data)
    print(total)
    try:
        print(hp.heap())
    except:
        pass

partial = r.fingerprints()
print('partial done')

assert len(partial) == len(reached)

for i in range(len(partial)):
    try:
        ps,pl,pp = partial[i]
        rs,rl,rp = reached[i]
        assert ps == rs
        assert pl == rl
        assert pp == rp
    except:
        print('partial', partial[i])
        print('reached', reached[i])
        raise

assert partial == reached

print('passed')
