#!/usr/bin/env python

from __future__ import print_function

import os
import sys

from rabin import set_min_block_size, set_max_block_size, set_average_block_size, split_file_by_fingerprints

set_min_block_size(1024)
set_max_block_size(2048)
set_average_block_size(1024)

beforefn = 'before.bin'
afterfn = 'after.bin'

filesizeM = 10
Mb = 1024*1024

os.chdir(os.path.dirname(sys.argv[0]))
os.system('rm -rf before after; mkdir before after')

os.chdir('before')
os.system("dd if=/dev/urandom of=%s bs=%d count=%d" % (
    beforefn, Mb, filesizeM))
split_file_by_fingerprints(beforefn)
beforefh = open(beforefn, 'rb')
os.chdir('..')


os.chdir('after')
beforefh.seek(int(filesizeM/2 * Mb))
data = beforefh.read()
afterfh = open(afterfn, 'wb')
afterfh.write(data)
beforefh.seek(0)
data = beforefh.read(int(filesizeM/2 * Mb))
afterfh.write(data)
afterfh.close()
split_file_by_fingerprints(afterfn)
os.chdir('..')

before = os.listdir('before')
before.remove(beforefn)
after = os.listdir('after')
after.remove(afterfn)

db = {}
def collect(series, fingerprint):
    poly = fingerprint
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

os.system('rm -rf before after')
print('passed')

'''
before.sort()
after.sort()

# assert len(before) == len(after) 

diffcount = 0
for i in range(len(before)):
    try:
        bs,bl,bp = before[i]
        As,al,ap = after[i]
        # assert bs == As
        assert bl == al
        assert bp == ap
    except:
        print('before', before[i])
        print('after', after[i])
        diffcount += 1

blocks = len(before)
print(blocks, diffcount)
assert diffcount < blocks*.8, diffcount

os.system('rm -rf before after')
print('passed')
'''
