# FridgeHub :ice_cube:
A smart and cool fridge simulator

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

# Install dependecies
- Install pistache
```
$ sudo add-apt-repository ppa:pistache+team/unstable
$ sudo apt update
$ sudo apt install libpistache-dev
```

- Install json parse library 
```
$ sudo apt-get install libjsoncpp-dev
```

- Install mosquitto
```
$ sudo apt-get install libmosquitto-dev
$ sudo apt-get install mosquitto
```

- Compile 
```
g++ main.cpp JsonClass.cpp Fridge.cpp Item.cpp -ljsoncpp -lpistache -lpthread -lmosquitto -o main
```
