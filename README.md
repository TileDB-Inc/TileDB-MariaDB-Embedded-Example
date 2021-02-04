# TileDB-MariaDB-Embedded-Example
Instructions and examples of using MariaDB embedded with the MyTile storage engine

## Docker
In the docker directory you will find a Dockerfile and directory containing an example program (C++). The Dockerfile follows the steps in the next section to build a docker image which runs the example program. The example program loads MyTile Embedded (An Embedded MariaDB instance with the TileDB Storage Engine), creates a table, inserts some data into it, and selects some data from it.  

To build and run the provided Dockerfile, first clone this repository:  
`git clone https://github.com/TileDB-Inc/TileDB-MariaDB-Embedded-Example`  
  
Next, change to the docker directory in the file tree:  
`cd TileDB-MariaDB-Embedded-Example/docker`  
  
Build the image and note the image id:  
`sudo docker build .`  
  
Run the container:  
`sudo docker run <image id>`  
  
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
