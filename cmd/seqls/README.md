seqls &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[![GitHub version](https://badge.fury.io/gh/justinfx%2Fgofileseq.svg)](https://github.com/justinfx/gofileseq/releases/latest)
========

A command-line utility, using [gofileseq](https://github.com/justinfx/gofileseq), to list file sequences.

Example sequence formats
---------------------------------

* /path/to/some/file_foo.0100.exr
* /path/to/some/file_foo.1-100#.jpg
* /path/to/some/file_foo.1-100@@@.tif
* relative_file.1-10,50-100x2#.tif
* file_without_frame.ext

Usage
---------

```bash
# list the current directory, with rolled up sequences
seqls

# *only* list sequences, and not single files without frame patterns
seqls -s 

# long listing of all sequences in current dir
seqls -l

# long listing, with absolute (full) paths
seqls -lf

# long listing, with absolute (full) paths, and human-readable file sizes
seqls -lfH

# long listing of all files (even those without frame #)
seqls -la

# recursively list all files from the current dir, down
seqls -ra

# list sequences in two locations 
seqls /some/absolute/path some/rel/path

# Pattern matching to ${base}${pad}${ext}
seqls /path/to/filesequence.#.exr 
seqls relative/path/to/files.#.jpg another/relative/set/of/files.#.exr
```
