GameJam Quick
=============

About
-----

A simple game made reeeeeeally quick for UFSC's 2nd GameJam.
You can check the submission in [in the itch.io
project](https://jptiz.itch.io/prawrtect-the-city).

Build
-----

### CMake

For CMake 3.13+:

```console
$ cmake -B build src -D CMAKE_BUILD_TYPE=Release
$ cmake --build build
```

After that, you can **copy ./build/app/app** executable to the current
directory:

```console
$ cp ./build/app/app .
```

Run
---

Run the `app` executable:

```console
$ ./app
```

Make sure you have `res/` on the same directory you're running the command.
