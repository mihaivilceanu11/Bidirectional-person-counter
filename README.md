# Bidirectional-person-counter
The main purpose of the system is to count people who are currently in a room. This is achieved by incrementing or decrementing a counter, depending on the movement of a person. For the detection system, there are two reflective IR sensors(KY-032), immune to light radiation, so there will not be any missintepretation due to the light level of the room. Initially developed using Arduino UNO, but adapted to Node-MCU in order to use IoT features.
The implimentation is really simple: If a sensor is activated, the algorithm will wait 150 ms before checking the second sensor to confirm that the person is passing.(It can be considered a debounce and a mean to determine the actual movement of the person(entering/exiting) ).

Key features:
Telegram message when the system comes online. 
SGP-30 CO2 and TVOC sensor, to monitor air quality depending on how many people are in the room.
The number of people is reported to thingspeak along with CO2 and TVOC levels.
OLED screen which gives the possibility to see the number of people live on the spot.
