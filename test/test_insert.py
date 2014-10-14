#!/usr/bin/env python

import os
import random

from rabin import Rabin, get_file_fingerprints, set_min_block_size, set_max_block_size, set_average_block_size

TARGET = 'test.bin'

filesizeM = 10
Mb = 1024*1024

os.system("dd if=/dev/urandom of=%s bs=%d count=%d" % (
    TARGET, Mb, filesizeM))
random.seed(open(TARGET, 'r').read(1024))

set_min_block_size(1024)
set_max_block_size(2048)
set_average_block_size(1024)

r = Rabin()

before = get_file_fingerprints(TARGET)
f = open(TARGET, 'r+')
data = f.read(filesizeM/2 * Mb)
r.update(data)
r.update('x')
data = f.read()
r.update(data)
after = r.fingerprints()

assert len(before) <= len(after) 

diffcount = 0
for i in range(len(before)):
    try:
        bs,bl,bp = before[i]
        As,al,ap = after[i]
        # assert bs == As
        assert bl == al
        assert bp == ap
    except:
        print 'before', before[i]
        print 'after', after[i]
        diffcount += 1

assert diffcount < 10, diffcount

os.unlink(TARGET)
print 'passed'
