# TileDB-MariaDB-Embedded-Example

Instructions and examples of using MariaDB Embedded with the MyTile storage engine  

There are two sections to this example project. The first section [docker](#docker) walks you through using the provided dockerfile to build the example project. This follows the same basic steps as the first section but provides an isolated, reproducible environment.

The second section [MyTile Embedded](#mytile-embedded) walks you through the process of building the example project locally. This assumes you are using an ubuntu linux instance. If you are on OSX or a different linux version your steps for the dependencies will differ.


## Docker
In the docker directory you will find a Dockerfile and directory containing an example program (C++). The Dockerfile follows the steps in the above section to build a docker image which runs the example program. The example program loads MyTile Embedded (An Embedded MariaDB instance with the TileDB Storage Engine), creates a table, inserts some data into it, and selects some data from it.

To build and run the provided Dockerfile, first clone this repository:

```
git clone https://github.com/TileDB-Inc/TileDB-MariaDB-Embedded-Example
```

Next, change to the docker directory in the file tree:  
```
cd TileDB-MariaDB-Embedded-Example/docker
```

Build the image and note the image id:
```
docker build .
```
  
Run the container:  
```
docker run <image id>
```
  
You should see the following output:
```
Got ERROR: "Could not open mysql.plugin table. Some plugins may be not loaded" errno: 2000
Got ERROR: "Can't open and lock privilege tables: Table 'mysql.servers' doesn't exist" errno: 2000
Got ERROR: "Can't open the mysql.func table. Please run mysql_upgrade to create it." errno: 2000
Affected rows: 1
Affected rows: 0
Affected rows: 3
>> 1	1	
>> 2	2	
>> 3	3
```

Note: the 3 errors are related to running the embedded server, and can be disregarded

## MyTile Embedded

The MyTile storage engine can be utilized as part of a MariaDB Embedded installation. MariaDB embedded allows your application to access database functionality without the need to run and communicate with a server application. The 3 main components required to build the embedded server are TileDB, MyTile, and MariaDB.  

### Build Dependencies

Build dependencies for MariaDB are required to be installed on the system. If you are using Ubuntu (as the dockerfile) you can install it with the apt command below. For other systems please see the [docs](https://docs.tiledb.com/mariadb/installation-from-source#prerequisites).
```
apt-get build-dep mariadb-server
```

### Building MariaDB

Next, clone the TileDB-MariaDB repository with the desired release tag located [here](https://github.com/TileDB-Inc/TileDB-MariaDB/releases/tag/0.7.0). This package will be built as part of the MariaDB Embedded build process. The current recommended version is `0.7.0`. `0.7.0` will also build and install TileDB `2.2.3` if it is not found on the system.

```
wget https://github.com/TileDB-Inc/TileDB-MariaDB/archive/0.7.0.tar.gz
tar xf 0.7.0.tar.gz
```

Now you can clone the MariaDB repository. Note that specific versions are required for compatability. Currently, we recommend MyTile `0.7.0` is built with MariaDB `10.4.17`. You can find the specified tagged release [here](https://github.com/MariaDB/server/tree/mariadb-10.4.17)

```
wget http://downloads.mariadb.org/rest-api/mariadb/10.4.17/mariadb-10.4.17.tar.gz
tar xf mariadb-10.4.17.tar.gz
```

A symbolic link to the MyTile directory must be setup in order for it to be pulled in to the MariaDB build. Inside the MariaDB code directory, we create the link under the `storage` directory. For example, if you cloned the MyTile repository above to `/tmp/TileDB-MariaDB`, run the following commands
```
ln -s $PWD/TileDB-MariaDB-0.7.0 $PWD/mariadb-10.4.17/storage/mytile
```

To build the embedded server with MyTile included, two specific build flags must be specified in the cmake command, `-DWITH_EMBEDDED_SERVER=ON` and `-DPLUGIN_MYTILE=YES`. We also need to specify a location for installation with `-DCMAKE_INSTALL_PREFIX`. The rest of the flags depend on your specific requirements. For example:  
```
mkdir mariadb-10.4.17/build
cd mariadb-10.4.17/build
cmake -DCMAKE_INSTALL_PREFIX=/mytile-install-dir -DWITH_EMBEDDED_SERVER=ON -DPLUGIN_INNODB=NO -DPLUGIN_INNOBASE=NO -DPLUGIN_TOKUDB=NO -DPLUGIN_ROCKSDB=NO -DPLUGIN_MROONGA=NO -DPLUGIN_SPIDER=NO -DPLUGIN_SPHINX=NO -DPLUGIN_FEDERATED=NO -DPLUGIN_FEDERATEDX=NO -DPLUGIN_CONNECT=NO -DPLUGIN_MYTILE=YES -DCMAKE_BUILD_TYPE=Release ..
```

The server can now be built from within the build directory:  
```
make -j$(nproc)
make install
```

### Using the Embedded Build

To use the embedded server, your application must have access to the library `libmysqld` located in the directory  
`/mytile-install-dir/lib`  

It will also need the header file `mysql/mysql.h` located in  
`/mytile-install-dir/include`  

Accordingly, be sure your application contains the following include directive:  
```
#include <mysql/mysql.h>
```

There are a few ways to instruct the compiler to use the correct pathing. One is to set the environment variables `CPATH` and `LIBRARY_PATH` accordingly. These can also be specified inline to the GCC compiler with the `-I` and `-L` options. If using one of these methods, be sure to specify the libary to link with in your GCC command:   
```
g++ application.cxx -o application -lmysqld
```  

If using cmake, you can instruct it to find the required files for you:  
```
find_path(MySQL_INCLUDE_PATH NAMES mysql/mysql.h)
find_library(MySQL_EMBEDDED_LIBRARIES NAMES mysqld)
...
target_include_directories(application PUBLIC ${MySQL_INCLUDE_PATH})
target_link_libraries(application ${MySQL_EMBEDDED_LIBRARIES})
```  

If using this method, be sure to pass the base MyTile installation path to cmake when building your application:  
```
cmake -DCMAKE_PREFIX_PATH=/mytile-install-dir ..
```

The project should build and link successfully. To see a full example, look at the `Dockerfile` and accompanying `example.cxx` program in the `docker` directory of this repository and the [previous section](#docker).   
