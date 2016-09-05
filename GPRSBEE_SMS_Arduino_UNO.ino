#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
const byte rx = 13;       //Pin de Rx que va conectado el modulo GPRS
const byte tx = 5;        //Pin de Tx que va conectado del modulo GPRS
String content = "";
char character;
String text;
int tiemp;
int tiemp2;
String stringuno;
AltSoftSerial GPRSBeeSerial(rx, tx);
SoftwareSerial XBee(2, 3); // Arduino RX, TX (XBee Dout, Din)     //Pin de Rx y Tx donde van conectado el Xbee coordinador
char numero_cel[] = "977792306"; // Variable donde va el numero de celular de destino (9xxxxxxxx).

void setup() { 
  Serial.begin(9600);
  XBee.begin(9600);
  GPRSBeeSerial.begin(9600);
  configuracion();
  pinMode(12, HIGH);  //Se usa el pin 12 para prender el modulo de GPRS (recordando que se prede por activacion externa de 5v)
}
void configuracion() {        //Configuracion estandar para las opciones de envio de SMS con el modulo GPRS.
  GPRSBeeSerial.println("AT+IPR=9600");
  Serial.println("AT+IPR=9600");
  delay(200);
  GPRSBeeSerial.println("AT+CMGF=1");
  Serial.println("AT+CMGF=1");
  delay(200);
}
void loop() {
if (XBee.available()){
character = XBee.read();       //Se lee lo que recibio el XBee y se guarda en esa variable (bits por bits)
delay(300);      //delay de seguridad
content.concat(character);     // Se concatena los bits recibidos para formar un solo texto 
tiemp = 1;          // Las variables tiemp y teimp2 ayudan a determinar el momento del envío del mensaje
tiemp2 = 1;
}
else {
  tiemp = 0;
}
if (tiemp == 0 && tiemp2 == 1){         //Aca se determina que una vez recivido y concatenado el mensaje, se realiza el envío
  enviar_mensaje();
  tiemp2 = 0;
  character = 0;            //Una vez realizado el envío, la variable character se vuelve 0
  content = "";             //Una vez realizado el envío, la variable content se vuelve 0
}
}

void enviar_mensaje()
{
  text = content.substring(16);           // Se pasa a la variable "text" todo lo existente en el texto concatenado despues de la posicion 16 
  GPRSBeeSerial.println("AT+CMGF=1");     //En el "void configuracion" ya fueron activadas estas opciones de envio y se vuelven a activar solo por precaucion
  Serial.println("AT+CMGF=1");
  delay(2000);
  GPRSBeeSerial.print("AT+CMGS=");       //En el "void configuracion" ya fueron activadas estas opciones de envio y se vuelven a activar solo por precaucion
  Serial.print("AT+CMGS=");
  GPRSBeeSerial.print((char)34);        //Char que representa (") ya que el numero debe ir entre comillas
  Serial.print((char)34);
  GPRSBeeSerial.print(numero_cel);
  Serial.print(numero_cel);
  GPRSBeeSerial.println((char)34);      //Char que representa (") ya que el numero debe ir entre comillas
  Serial.println((char)34);
  delay(300);
  GPRSBeeSerial.println(text);      //Aca en teoria va la variable del texto que se envia por mensaje SMS.
  Serial.println(text);       //Por las pruebas realizadas, la variable del texto que se envia por SMS va en esta funcion.
  GPRSBeeSerial.println((char)26);    //Char que representa el "Control + Z" que activa el envío del SMS
  Serial.println((char)26);     //Char que representa el "Control + Z" que activa el envío del SMS
  delay(1000);
  Serial.println("Mensaje Enviado");
}
