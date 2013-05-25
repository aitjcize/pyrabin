#!/usr/bin/env python

from rabin import Rabin, get_file_fingerprints, set_min_block_size, set_max_block_size, set_average_block_size

TARGET = 'test.bin'

set_min_block_size(1024)
set_max_block_size(2048)
set_average_block_size(1024)

def block_reached(start, length, fingerprint):
    print '(%s, %s, %s)' % (start, length, fingerprint)

r = Rabin()
r.register(block_reached)

with open(TARGET, 'r') as f:
    data = f.read()
    half = len(data) / 2
    r.update(data[:half])
    r.update(data[half:])

partial = r.fingerprints()
gold = get_file_fingerprints(TARGET)

print gold
print partial == gold
