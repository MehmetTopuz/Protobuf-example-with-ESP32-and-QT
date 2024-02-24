# How to Build ESP32 Project

Firstly, I created this project using the Platformio extension in VS Code. In this project, the ESP32-WROOM-32D series is used as the ESP32. Therefore, it is configured as ```board = upesy_wroom``` in the platformio.ini file. You can simply use VS Code to compile the project (if the Platformio extension is installed). Additionally, you can follow the steps below to compile from the command line.

```cd ESP32_Protobuf```

## Build

```C:\Users\Topuz\.platformio\penv\Scripts\platformio.exe run --environment upesy_wroom```

## Upload

```C:\Users\Topuz\.platformio\penv\Scripts\platformio.exe run --target upload --environment upesy_wroom```

# How to Build QT Project

You can use the Qt Creator IDE to compile and run the Qt project. You should only compile and run the project in Debug mode. The static library of protocol buffers is not available in this project for the Release mode (libprotobuf.lib).
Alternatively, you can use ```qmake``` from the command line to compile.
