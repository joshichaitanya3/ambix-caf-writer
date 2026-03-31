# ambix-caf-writer
A toy ambisonics encoder and AmbiX-basic CAF writer in C++-23. Requires a compatible compiler.

Uses the [AudioFile](https://github.com/adamstark/AudioFile) library (installed separately) to import WAV files and the [CLI11](https://github.com/CLIUtils/CLI11) library (via FetchContent) for CLI arguments.

Instructions to build:

```
git clone https://github.com/joshichaitanya3/ambix-caf-writer.git
cd ambix-caf-writer
cmake -B build
cmake --build build
```

To run:

```
./build/monoToAmbix 
    -i <full path to mono wav file>
    -a <azimuth>
    -e <elevation>
    -d <ambisonic_degree>
    -o <optional full path to output CAF file>
```

The tests are written using [GoogleTest](https://github.com/google/googletest.git). 

The output CAF files for upto 4th-degree ambisonics are tested by importing them into the [Reaper](https://www.reaper.fm/index.php) DAW and applying the `BinauralDecoder` [IEM plugin](https://plugins.iem.at/).
