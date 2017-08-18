#!/usr/bin/env python

from __future__ import print_function

import os
import random

from rabin import Rabin, get_file_fingerprints, set_min_block_size, set_max_block_size, set_average_block_size

TARGET = 'test.bin'

filesizeM = 10
Mb = 1024*1024

os.system("dd if=/dev/urandom of=%s bs=%d count=%d" % (
    TARGET, Mb, filesizeM))
random.seed(open(TARGET, 'rb').read(1024))

set_min_block_size(1024)
set_max_block_size(2048)
set_average_block_size(1024)

r = Rabin()

before = get_file_fingerprints(TARGET)
f = open(TARGET, 'rb+')
f.seek(int(filesizeM/2 * Mb))
data = f.read()
r.update(data)
f.seek(0)
data = f.read(int(filesizeM/2 * Mb))
r.update(data)
after = r.fingerprints()

db = {}
def collect(series, fingerprint):
    start,length,poly = fingerprint
    db.setdefault(poly, [])
    db[poly].append((series, fingerprint))

for fingerprint in before:
    collect('before', fingerprint)

for fingerprint in after:
    collect('after', fingerprint)

diffcount = 0
for entry in db.values():
    if len(entry) != 2:
        diffcount += 1
        print(entry, len(entry))

print(len(db), diffcount)
assert diffcount < len(before)*.01, diffcount

os.unlink(TARGET)
print('passed')
