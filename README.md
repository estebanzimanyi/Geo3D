Geo3D
=====
Geo3D is an extension for PostgreSQL to support 3-dimensional geometries.

Requirements
------------
 - Linux (other UNIX-like systems may works, but remain untested)
 - PostgreSQL  (a relatively recent version, along the development package if applicable to your Linux distribution)
 - CMake >= 2.8

Building & installation
-----------------------
Here is the gist:
```bash
$ git clone https://github.com/estebanzimanyi/Geo3D.git
$ cd Geo3D
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```
Note that there have been cases where the build script wouldn't correctly detect PostgreSQL's extension directory (usually `/usr/share/postgresql/extension`). If that happens, you'll need to install the relevant files manually (`geo3d.control` and `geo3d-x.y.z.sql`).

License
-------
Geo3D is provided under the PostgreSQL license.
