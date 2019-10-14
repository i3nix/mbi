# mbi

[CMake 3.8](https://cmake.org/download/) required!  
C++17 required!  
Contains [yaml-cpp](https://github.com/jbeder/yaml-cpp) and [rabbitmq-c](https://github.com/alanxz/rabbitmq-c) libraries  

# Build

Make:  
```
mkdir build  
cd build  
cmake ..  
```

MSVS:
 - build generated mbi-proj solution  
 
Unix:
 - idk yet  
 
MacOS:
 - idk yet  
 
Other:
 - not tested, might work  
 
# Usage

```
mbi
```
 composes all the files in ```migration``` directory in lexicographical order into final.yml and sends it to RabbitMQ server  

```
mbi <file1> <file2> ... <fileN>
```
 sends all the files to RabbitMQ server  
 
mbi uses AMQP_URL environment variable to connect to RabbitMQ server
