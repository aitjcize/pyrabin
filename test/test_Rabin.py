#!/usr/bin/env python

from rabin import Rabin, get_file_fingerprints

TARGET = 'test.bin'

r = Rabin()

with open(TARGET, 'r') as f:
    data = f.read()
    half = len(data) / 2
    r.update(data[:half])
    r.update(data[half:])

partial = r.fingerprints()
gold = get_file_fingerprints(TARGET)

print partial == gold
