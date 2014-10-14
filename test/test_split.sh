#!/bin/bash

cleanup() {
  rm -rf *.blk file.bin rabin.bin ordered.lst
}

echo -n '[*] Generating a random file "file.bin" ... '
dd if=/dev/urandom of=file.bin bs=1024 count=100 > /dev/null 2>&1
echo 'done'

echo -n '[*] Splitting file.bin to rabin chunks ... '
python << EOF
from sys import stdout
from rabin import split_file_by_fingerprints
result = split_file_by_fingerprints("file.bin")
print 'done'

stdout.write('[*] Reconstructing "rabin.bin" with rabin chunks ...')
with open('ordered.lst', 'w') as f:
    for start, length, fingerpint, filename in result:
        f.write(filename + "\n")
EOF

cat `cat ordered.lst` > rabin.bin
echo 'done'

echo -n '[*] Checking if sha1sum of file.bin and rabin.bin are identical ... '
sha1_orig=`sha1sum file.bin | awk '{ print $1 }'`
sha1_recst=`sha1sum rabin.bin | awk '{ print $1 }'`

if [ "$sha1_orig" == "$sha1_recst" ]; then
  cleanup
  echo "passed"
  exit 0
else
  cleanup
  echo "failed"
  exit 1
fi
