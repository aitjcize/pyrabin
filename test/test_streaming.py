#!/usr/bin/env python

import os
import random
import resource
import sys

from rabin import Rabin, get_file_fingerprints, set_min_block_size, set_max_block_size, set_average_block_size

stream_bs = 1024 * 1024 
stream_count = 100
stream_len = stream_bs * stream_count
max_mem = stream_len/2

resource.setrlimit(resource.RLIMIT_AS, (max_mem,-1))
# print resource.getrlimit(resource.RLIMIT_AS)

TARGET = 'test.bin'
os.system("dd if=/dev/urandom of=%s bs=%d count=%d" % (
    TARGET, stream_bs, stream_count))
random.seed(open(TARGET, 'r').read(1024))

max_blocksize = random.randint(512, max_mem/10)

set_min_block_size(max_blocksize/10)
set_max_block_size(max_blocksize)
set_average_block_size(max_blocksize/5)

reached = []
def block_reached(start, length, fingerprint):
    # print '(%s, %s, %s)' % (start, length, fingerprint)
    reached.append((start, length, fingerprint))

r = Rabin()
r.register(block_reached)

# from guppy import hpy; hp=hpy()

with open(TARGET, 'r') as f:
    while True:
        size = random.randint(1,max_blocksize*2)
        data = f.read(size)
        if len(data) == 0:
            break
        r.update(data)
        # print hp.heap()

partial = r.fingerprints()
print 'partial done'
gold = get_file_fingerprints(TARGET)

assert len(gold) == len(partial) == len(reached)

for i in range(len(gold)):
    try:
        gs,gl,gp = gold[i]
        ps,pl,pp = partial[i]
        rs,rl,rp = reached[i]
        assert gs == ps == rs
        assert gl == pl == rl
        assert gp == pp == rp
    except:
        print 'gold   ', gold[i]
        print 'partial', partial[i]
        print 'reached', reached[i]
        raise

assert partial == gold == reached

os.unlink(TARGET)
print 'passed'
