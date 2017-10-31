BioBundle
=========

Collection of simple programs for biological data. The following programs exist:

seqExtract - Allows to extract sequences from a larger set of sequences.

isoformCleaner - Removes isoforms from as sequence set and only keeps the longest one.

seqSetInfo - Calculates basic statistics of a sequence set.


Installation
------------

Dependencies:
- cmake (https://cmake.org)
- boost (http://www.boost.org)
    - program_options
    - filesystem
    - iostreams
- compiler supporting c++11 (g++ 4.9 or higher, 4.8 will complile but options related to regular expression will fail)

To compile the programs run the following commands in the main directory:

```bash
mkdir build
cd build
cmake ..
make
```

The programs can than be copied anywhere or included in the path.


Manual
------

Please check the wiki page for a detailed manual:
https://git.ckemena.de/ckemena/BioBundle/wikis/home


Bugs and Suggestions
--------------------

Programs are tested but as in all programs errors may still exist. In case you find one, or if you have any suggestions, please write an email to carstenk - at - posteo.de.