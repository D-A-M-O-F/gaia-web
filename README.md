# gaia-web

## How to build and run

In order to download and build the project you will need the following tools

- c++ compiler
- git
- cmake
- make

With the above pre-requisite then proceed with the following:

```bash
$ git clone https://github.com/D-A-M-O-F/gaia-web.git
$ cd gaia-web
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF -DBUILD_UNITTESTS=OFF ..
$ make
$ . ../scripts/makelinks.sh
$ ./gaia-web
```

Well done and and you should application listening on port 8080, so to check if everithing is working properly try to open your browser at `http://127.0.0.1:8080/` .
