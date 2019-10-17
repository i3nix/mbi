# mbi

[CMake 3.8](https://cmake.org/download/) required!  
C++17 required!  
Contains [yaml-cpp](https://github.com/jbeder/yaml-cpp) and [rabbitmq-c](https://github.com/alanxz/rabbitmq-c) libraries  

# Build

### MSVS:  
```
mkdir build  
cd build  
cmake ..  
```
and build generated mbi-proj solution  
 
### g++(g++-9 or later):  
I was used to tell CMake to use correct g++ version, maybe it will not be necessary for you  
```
mkdir build  
cd build  
cmake .. -DCMAKE_CXX_COMPILER=g++-9
```
### Clang++:  
Here I should make some preparations, which can be different. Of cource, maybe it also will not be necessary for you.  
Install LLVM(probably 7.0.1+, tested for 9.0.0), it can take some time:  
```
brew install llvm  
```
After this modify some environment variables:  
```
export LDFLAGS="-L/usr/local/Cellar/llvm/9.0.0/lib"  
export CPPFLAGS="-I/usr/local/Cellar/llvm/9.0.0/include"  
export PATH="/usr/local/Cellar/llvm/9.0.0/bin:$PATH" >> ~/.bash_profile"  
export MACOSX_DEPLOYMENT_TARGET=10.15  
```
And now build:
```
mkdir build  
cd build  
cmake -DCMAKE_CXX_COMPILER=/usr/local/Cellar/llvm/9.0.0/bin/Clang++ ..  
```

For any other compiler it also might work, but not for sure  

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
