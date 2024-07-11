# FROST-Meltybrain
Welcome to the FROST Robotics respository for the __Brainfreeze__ Meltybrain combat robot.\
This repo is where tests, codes, derivations, and important resources will be located.

FROST Robotics is the official combat robotics club of the University of Connecticut.

## TODO
### _For general upkeep_
- [x] Clean up the repository some more :money_with_wings:
- [ ] Clean up stray branches, deprecate them?
  - [ ] What diff between main, devel, test branches?

### _For the actual robot_

#### Software
- [ ] Finish movement handling (_input:_ accelerometer data, RC controls ; \
    _output:_ motor commands)
  - [ ] Update necessary resources in MD, include derivation
- [x] Finish accelerometer handling (_input:_ accelerometer i2c ; \
    _output:_ accelerometer data)
- [ ] Finish motor handling (_input:_ motor commands, battery voltage? ; \
    _output:_ None)
- [x] Finish RC comms handling (_input:_ rx serial ; _output:_ RC controls) 
- [ ] Other stuff to add

#### Hardware
- [ ] Meltybrain body full CAD
  - At NHRL June 2024, "The Greatest Challenge" uses 10" diameter aluminum pipe\
    fitting (prefab), with little actual machining

## Useful links/resources
- _Shift + CTRL + V_ to preview .md in VSCode
- [Trello board](https://trello.com/b/osOCW6Ca/meltybrain-frost-robotics)
- [Spencer's Hardware Blog](https://www.swallenhardware.io/battlebots/2018/4/28/halo-a-first-adventure-in-combat-robotics-overview)
- [OpenMelt2 GitHub :star:](https://github.com/nothinglabs/openmelt2)
#### List of current components/devices
- Microcontroller: Arduino Uno/Nano (for dev) and Teensy 4.0
- Accelerometer: H3LIS331DL
- ESCs: Readytosky 40A (BLHeli32)
- Voltage Regulator: 5V Regulator and Buck Converter
- Gyroscope: None
- Battery: whatever Spencer has
- BLDC Motors: Drone Motors (from that SD drone project)
- TX/RX: whatever Spencer has (Flysky I6x TX + IA6B RX)


## Notes
- Might move this section to separate .txt or .md files for each subsection