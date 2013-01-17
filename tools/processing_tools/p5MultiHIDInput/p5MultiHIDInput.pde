import procontroll.*;
import net.java.games.input.*;
import java.io.*;

import processing.serial.*;

/**
 * oscP5sendreceive by andreas schlegel
 * oscP5 website at http://www.sojamo.de/oscP5
 */


/**
 * procontroll by Christian Riekoff
 * procontroll website: http://www.creativecomputing.cc/p5libs/procontroll/
*/

import oscP5.*;
import netP5.*;
import processing.serial.*;
 
 
ControllIO controll;
ControllDevice device;
ControllDevice joypad;
ControllStick stick1;
ControllStick stick2;
ControllButton button;
 
 
 Serial myPort;        // The serial port
 float inByte; 


OscP5 oscP5;
NetAddress myRemoteLocation;

slider vectorX;
slider vectorY;
slider vectorZ;

boolean bUseArduino=false;

//prefix is the indicator for the sensor number
int prefix1 = 1;
//flag for the end of the signal
boolean myCatch1 = true;
 

void setup() {
  size(600,400);
  frameRate(25);
  /* start oscP5, listening for incoming messages at port 12000 */
  //we dont really need to listen, so we leave at port 12000
  oscP5 = new OscP5(this,12000);
  
  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  myRemoteLocation = new NetAddress("127.0.0.1",31843);
  
  vectorX=new slider(200,20);
  vectorY=new slider(320,20);
  vectorZ=new slider(440,20);

 

  controll = ControllIO.getInstance(this);
  controll.printDevices();
  joypad = controll.getDevice(3);

  println( joypad.getNumberOfButtons() );
  println( joypad.getNumberOfSticks() );
  println( joypad.getNumberOfSliders() );
}

void draw() {
  background(128);  
  
  vectorX.draw();
  vectorY.draw();
  vectorZ.draw();

  sendUDP();
}


void sendUDP() {
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage myMessage = new OscMessage("/pilot/float/vector3f");
  
    myMessage.add(vectorX.value/50.0); /* add a float to the osc message */
 
    myMessage.add(vectorX.value/2.0 -5.0); /* add a float to the osc message */
    myMessage.add(vectorZ.value/2.0); /* add a float to the osc message */
    myMessage.add(vectorY.value/1.0 - 10); /* add a float to the osc message */
  /* send the message */
  oscP5.send(myMessage, myRemoteLocation); 
}


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
