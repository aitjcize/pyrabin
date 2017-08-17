#!/usr/bin/env python

from __future__ import print_function

import os
import random

from rabin import Rabin, get_file_fingerprints, set_min_block_size, set_max_block_size, set_average_block_size

TARGET = 'test.bin'
os.system("dd if=/dev/urandom of=%s bs=1024k count=10" % TARGET)
random.seed(open(TARGET, 'rb').read(1024))

set_min_block_size(1024)
set_max_block_size(2048)
set_average_block_size(1024)

r = Rabin()

before = get_file_fingerprints(TARGET)
fh = open(TARGET, 'rb+')
fh.seek(1024*1024*5)
fh.write(b'x')
fh.close()
after = get_file_fingerprints(TARGET)

assert len(before) == len(after) 

diffcount = 0
for i in range(len(before)):
    try:
        bs,bl,bp = before[i]
        As,al,ap = after[i]
        assert bs == As
        assert bl == al
        assert bp == ap
    except:
        print('after', after[i])
        print('before', before[i])
        diffcount += 1

assert diffcount == 1, diffcount

os.unlink(TARGET)
print('passed')
