# CppSolyanka
First project on c++

---

___Installing dependencies___
```
sudo apt update
sudo apt install cmake
```

___Buil project___
```
cd CppSolyanka
mkdir build && cd build
cmake ..
make -j
```

___Server start___
```
cd CppSolyanka/build/src/server
./server
```

___Client start___
```
cd CppSolyanka/build/src/server
./client
```
___Available commands___
```
registration <name> <surname> <patronymic> <pasport> <password>
login <pasport> <password>
```
