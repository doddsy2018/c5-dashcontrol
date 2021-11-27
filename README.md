# Corvette c5 Dash Control

## About

Arduino code to control Corvette C5 air core dash gauges

Using SimHub custome serial feature to direct system output values from game

## Messgae Code Syntax

```C
'{"speed":' + [SpeedMph] 
+ ',"rpm":' + [Rpms]
+ ',"fuel":' + [Fuel] 
+ ',"oiltemp":' + [OilTemperature] 
+ ',"oilpres":' + [OilPressure]
+ '}\n'
```

## Sample message

```json
{"speed":9.07,"rpm":902.692138671875,"fuel":67.3903065919876,"oiltemp":81.22,"oilpres":7.39}
```

## References

[Air_core_gauge Wiki](https://en.wikipedia.org/wiki/Air_core_gauge)
