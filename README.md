# CANComLib

CANComLib is an small and open source library written in C used by Android apps to send and receive CAN messages via the SocketCAN Interface of the Linux kernel. This gives Android apps the oppertunity to communicate with devices connected to the CAN bus.

It was developed in 2011 and due to some ongoing questions if the source was available for the public, the library was released as open source software in 2014.

The library was mentioned in the following talks:
* M. Uelschen, R. Rakers: "Einsatz von Android als Plattform im Fahrzeug: Möglichkeiten und Herausforderungen", 17. VDE/ITG Fachtagung Mobilkommunikation, Osnabrück, May 2012 (http://goo.gl/UqJFl9)
* M. Uelschen, R. Rakers: "Android for Automotive: Yes, we CAN", droidcon.2012, Berlin, March 2012 (http://goo.gl/DjikOq)

## Library usage
The lib has an easy to use interface. At first initialize() must be called. After that the initialized socket can be used by receive_can(), send_can() and send_can_testframe(). The socket can be closed by calling terminate().
* int initialize();
* int terminate();
* struct can_frame receive_can(struct can_filter filter);
* int send_can(struct can_frame sendFrame);
* void send_can_testframe();

## Usage from Android
You will need a Java class with native functions to make native calls. Something like this will do the job:
```java
public class CANCom {
  // load the library - name defined in Android.mk 
  static {
    System.loadLibrary("cancomandroid");
  }
  
  // declare the native method - must be implemented in C
  public native int initialize();
  public native int terminate();
  public native CANMsg receiveCAN(CANFilter filter);
  public native int sendCAN(CANMsg canmsg);
}
```
I loaded and linked the CANComLib in the native initalize() call at time of app startup (see Library usage example). There was a thread in the app that repeatedly called receiveCAN() to pick up all CAN messages from the bus. The native receiveCAN() function created Java objects of the CANMsg class to return them.

## Building
The lib can be build with the delivered Makefile. You will have to alter some path variables and probably some other things to fit your needs. The Makefile was build in 2011 to compile the library for ARM based Android systems. I'm pretty sure it is outdated and will not work out of the box for the latest available NDKs. Sorry for that ;-)

## Installation
I installed the library with `make install`. By this the lib and some scripts were copied via ADB to the Android system. It is very likely that you will have to adjust this target as well to fit your requirements.

## Library usage example
It was designed to be loaded/linked dynamically by an Android app. The principle of loading/linking is shown in the delivered test program in *testlib/main.c*. This programm can be build be with `make test`.

## Requirements
The library opens the CAN socket *can0*. Therefore a running CAN device named *can0* must be upped in the linux kernel.
You can change the name of the CAN socket in *src/cancom.h*.
