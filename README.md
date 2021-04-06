# FridgeHub :ice_cube:
A smart and cool fridge simulator

# Install dependecies
- Install pistache
```
git clone --recurse-submodules https://github.com/pistacheio/pistache.git
cd pistache 
mkdir build && cd build 
cmake -DCMAKE_BUILD_TYPE=Release .. 
make 
sudo make install
```

- Install json parse library 
```
sudo apt-get install libjsoncpp-dev
```

- Install mosquitto
```
sudo apt-get install libmosquitto-dev
sudo apt-get install mosquitto
```

- Compile 
```
g++ main.cpp JsonClass.cpp Fridge.cpp Item.cpp -ljsoncpp -lpistache -lpthread -lmosquitto -o main
```

# Echipa
Alexiu Adrian-Stefan
<br />
Bondoc Gabriel
<br />
Dinescu Bogdan-George
<br />
Masgras Vlad-Florian
<br />
Sandu Alexandru-Cristian
