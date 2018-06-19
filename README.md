# PIR Motion Detection

## Setup Steps
### 1. Upload Traffic.ino to PIR Sensor
### 2. Change the WIFI SSID and Password.
### 3. Change the racknumber in the apiserver string that looks like :
```
http://smartrackapi.herokuapp.com/api/traffic/${racknumber will go here}
```
### 4. Make sure you connect pir sensor to raspberry pi of that rack if you need to debug it remotely.

## Wire Connections
### Wiring Standard

| Wire Color | Meaning |
| ---------- | -------- |
| Black | GND |
| RED | VCC , VIN , +5 |
| ANY COLOR | DATA |

### PIR To Node Wiring

| PIR | WIRE | NODE |
| ---- | ----- | ----- |
| VCC | RED | VIN |
| GND | BLACK | GND |
| OUT | ANY COLOR | D7 |
