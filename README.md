# PyRabin
A python module for generating [rabin fingerprints](http://en.wikipedia.org/wiki/Rabin_fingerprint).

The C extension implementation is based on [rabin-fingerprint-c](https://code.google.com/p/rabin-fingerprint-c/) with some bugs fixed.

## Features
* Fast fingerprint generation.
* Split file into blocks according to the rabin fingerprints.
* Window/Chunk size is configurable.

## API
### Module methods
* `set_prime(integer)`
  - Set Rabin polynomial prime, default is 3.
* `set_window_size(integer)`
  - Set Rabin polynomial sliding window size, default is 48 bytes.
* `set_max_block_size(integer)`
  - Set Rabin polynomial max block size, default is 64K bytes.
* `set_min_block_size(integer)`
  - Set Rabin polynomial min block size, default is 2K bytes
* `set_average_block_size(integer)`
  - Set Rabin polynomial average block size, average is 8K bytes.
* `get_file_fingerprints(filename)`
  - Returns a list of tupple, each tuple represents (offset, length, fingerprint) 
* `split_file_by_fingerprints(filename)`
  - Returns a list of tupple, each tuple represents (offset, length, fingerprint, filename).
  - The default filename is sha1sum of the chunk with '.blk' suffix.

### class Rabin
* `register(callback)`
  - Register a callback for calling when a block is reached
  - The callback is called with three parameters: (offset, length, fingerprint)
* `update(data)`
  - Update Rabin fingerprint list by adding a block of data
* `clear()`
  - Clear fingerprints and reset status
* `fingerprints()`
  - Return fingerprints of all data passed by the update method

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
