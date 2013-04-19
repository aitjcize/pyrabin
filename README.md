# PyRabin
A python module for generating [rabin fingerprints](http://en.wikipedia.org/wiki/Rabin_fingerprint) of a file.

The C extension implementation is based on [rabin-fingerprint-c](https://code.google.com/p/rabin-fingerprint-c/) with some bugs fixed.

## Features
* Fast fingerprint generation.
* Split file into blocks according to the rabin fingerprints.
* Window/Chunk size is configurable.

## API
* `get_file_fingerprints(filename, prime=3, window_size=48, max_block_size=65536, min_block_size=2048, avg_block_size=4096)`
  - Returns a list of tupple, each tuple represents (offset, length, fingerprint) 
* `split_file_by_fingerprints(filename, prime=3, window_size=48, max_block_size=65536, min_block_size=2048, avg_block_size=4096)`
  - Returns a list of tupple, each tuple represents (offset, length, fingerprint, filename).
  - The default filename is sha1sum of the chunk with '.blk' suffix.

## Examples
* See test/test.sh for example

## Bugs
* Please report bugs / mis-formatted pages to the github issure tracker.

## Contributing
1. Fork it
2. Create your feature branch (git checkout -b my-new-feature)
3. Commit your changes (git commit -am 'Add some feature')
4. Push to the branch (git push origin my-new-feature)
5. Create new Pull Request
