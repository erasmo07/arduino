//librerias
//libreria del motor paso a paso Stepper Motor
#include <AccelStepper.h>
#include <Stepper.h>
//Libreria Infrarojo, lee los comandos enviados por el control, las cuales de definen mas abajo.
#include <IRremote.h>
//Libreria de la pantalla LCD
#include <LiquidCrystal.h>

// declaraciones de Pines
// pantalla LCD
 #define RS 30
 #define ENABLE 31
 #define D4 25
 #define D5 24
 #define D6 23
 #define D7 22
 
LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);
//boton encendido
#define BI      8
#define LI      13   
// Stepp Motor pin definitions
#define motorPin1  9     
#define motorPin2  10    
#define motorPin3  11  
#define motorPin4  12    
//Led RGB pin definitions
#define RECV_PIN  4
#define R_PIN  7
#define G_PIN  6
#define B_PIN  5
//codificacion hexadecimal del control
#define ON        0xFFA25D
#define MODE      0xFF629D
#define SOUND     0xFFE21D
#define PLAY      0xFF22DD
#define PRE       0xFF02FD
#define NEXT      0xFFC23D
#define EQ        0xFFE01F
#define MAS       0xFF906F
#define MENOS     0xFFA857
#define FLECHA    0xFF9867
#define USD       0xFFB04F
#define BT0       0xFF6897
#define BT1       0xFF30CF
#define BT2       0xFF18E7
#define BT3       0xFF7A85
#define BT4       0xFF10EF
#define BT5       0xFF38C7
#define BT6       0xFF5AA5
#define BT7       0xFF42BD
#define BT8       0xFF4AB5
#define BT9       0xFF52AD

//boton de inicio
#define INI 3 // 
int inic=0;// variable que determina si se llego al inicio

//incicializacion de la variable controladora del stepp motor
#define MEDIOPASO 8 // variable que defini el impulso al motor, para que este lo simule como un paso.
AccelStepper stepper1(MEDIOPASO, motorPin4, motorPin2, motorPin3, motorPin1); // Aqui se declara donde esta conectado el motor al arduino.

//variables para el control de tiempo
long previousMillis = 0;        
int segundos=0;
int minutos=0;
int horas=0;
//variables led
unsigned long rgb = 0;
byte r,g,b;

//variables control
IRrecv irrecv(RECV_PIN);
decode_results results;

//banderas
int corre =1; // Esta determina cual de las dos librerias es la que se utilizara durante la ejecucion del programa.
//variables del boton de inicio (Banderas)
int estado=0; // guada el estado actual del boton
int estadoate=0; // guarda el estado anterior del boton
int unic=0; //variable que determina si el boton esta ensendido o apagado
int donde=0;//variables de configuracion por control
int digitos=0; //variable 
int confi=0;//variable de configuracion de la hora
int mover_a = 0;
int leonHora=7;
//movimiento a pasos
const int stepsPerRevolution = 10; // el impulso que necesita para dar un paso.
Stepper myStepper(stepsPerRevolution, motorPin1,motorPin3,motorPin2,motorPin4);// creamos la instacia del metodo Stepper que se llama myStepper.
int velocidadPasos =60;// aqui se define la velocidad de los pasos del motor

void setup()
{
    myStepper.setSpeed(velocidadPasos);// aqui se le configura la velocidad al motor
    
    //metodos de la libreria acelerado del stepper esto es diferente al myStepper
    //metodo que difine la cantidad de pasos a mover 
    // estos son parametros obligatorios y predeterminados que debe tener el motor.
    stepper1.moveTo(1150);
    //metodo que define la velocidad maxima que se puede mover
    stepper1.setMaxSpeed(1000);
    //define la aceleracion que tiene de un de un punto a otro
    stepper1.setAcceleration(1000);
   //aqui acaba los parametros predeterminados.
   
    //velocidad incial de los pasos
    stepper1.setSpeed(0);
    // Inicializamos el receptor
    irrecv.enableIRIn(); 
    
    pinMode(R_PIN, OUTPUT);   
    pinMode(G_PIN, OUTPUT);   
    pinMode(B_PIN, OUTPUT);  
    //pemite imprimir contenido en el monitor serial
    //Serial.begin(9600); 
    
   //definicion de los pines del boton INICIO
   pinMode(LI, OUTPUT);//declaracion del estado del salida del pin 4
   pinMode(BI, INPUT);//declaracion del estado de entrada del pin 2
   
   //Configuracion de la LCD
   lcd.begin(16, 2);
   lcd.print("Configarar hora");
}


void loop() {
//*********************************************************************\
jz
    if (irrecv.decode(&results)){
    //determina que la tecla no se quedo precionada
      if ( results.value != 0xFFFFFFFF) {
      //ejecuta la accion deacuerdo a la tecla precionada
        switch (results.value) {
             case ON : if(inic==1){ unic= 1-unic; RGB(0x0000FF00);} break;
        }
      
        analogWrite(R_PIN,r);
        analogWrite(G_PIN,g);
        analogWrite(B_PIN,b);
      
      }

      irrecv.resume(); 
    } 
    
//*********************************************************************

  if(confi==0){ 
    RGB(0x000000FF); 
    analogWrite(R_PIN,r);
    analogWrite(G_PIN,g);
    analogWrite(B_PIN,b);
  }

  //error de falta de hora
  
  if(confi==1)
  {
    determinarHora(); 
  }
  hora();
  
  if(inic==0)
  {
    inicio();
  }

  estado = digitalRead(BI);
  //lee el pin 2 y determina si esta ensendido o pagado
 

  //pregunta si el estado ensendio y el estado anterior era apagado
  if(estado==HIGH && estadoate==LOW&&inic==1){
    unic= 1-unic;//se le resta a 1 lo que tenia unic(esto variara su valor segun se precione el boton, si se preciona una vez valdra 1 y si lo preciona de nuevo valdra 0)
    delay(LI);// retraso de 20 mili segundos,  para evitar el revote del Boton
  }

  estadoate = estado;// estado anterior pasa a ser estado actual

 if(unic==1) {
   // si el contador marca 1 esta ensendido sino esta apagado
   digitalWrite(LI,HIGH);//envia energia alta al pin 4 (ensiende)
   //metodo de repeticion
   //pregunta si se preciono una tecla
  
  //-----codigo de configuracion primaria-----
  if(donde==0)
  {
    if (irrecv.decode(&results)) 
    {
    //determina que la tecla no se quedo precionada
      if ( results.value != 0xFFFFFFFF) 
      {
      //ejecuta la accion deacuerdo a la tecla precionada
        switch (results.value) {
          //acciones
             case MODE          : 
             RGB(0x00FF0000);
             confi=1;
             donde =1;
             
             break;
             
             case ON            : if(confi==1){RGB(0x0000FF00);} break;
             
             case SOUND         : if(confi==1){r=0;g=0;b=0;} break;
             /*case PLAY          : if(confi==1){stepper1.setSpeed(0); mover_a=0;}break;
             case PRE           : if(confi==1){stepper1.setSpeed(1000); corre =1; mover_a = 1; } break;
             case NEXT          : if(confi==1){stepper1.setSpeed(-1000); corre =1; mover_a = 1; } break;
             case EQ            : if(confi==1){stepper1.setSpeed(-1000);corre =0;  } break;*/         

        }
        //escribe el color del led
        analogWrite(R_PIN,r);
        analogWrite(G_PIN,g);
        analogWrite(B_PIN,b);
      }
      // Receive el proximo valor del boton pulsado
      irrecv.resume(); 
    } 
  }
 //-----codigo de configuracion de hora--------
 if(donde==1)
 { 
     if (irrecv.decode(&results)) 
     {
    //determina que la tecla no se quedo precionada
      if ( results.value != 0xFFFFFFFF) 
      {
      //ejecuta la accion deacuerdo a la tecla precionada
        switch (results.value) {

             case BT0            : hotaconf(0); break;
             case BT1            : hotaconf(1); break;
             case BT2            : hotaconf(2); break;
             case BT3            : hotaconf(3); break;
             case BT4            : hotaconf(4); break;
             case BT5            : hotaconf(5); break; 
             case BT6            : hotaconf(6); break;
             case BT7            : hotaconf(7); break;
             case BT8            : hotaconf(8); break;
             case BT9            : hotaconf(9); break;
        }
      }
      // Receive el proximo valor del boton pulsado
      irrecv.resume(); 
    } 
 }
 if(mover_a==1)
 {
    //Change direction when the stepper reaches the target position
    if (stepper1.distanceToGo() == 0) 
    {
      stepper1.moveTo(-stepper1.currentPosition());      
    }
    
    if(corre==0)
    { 
      stepper1.run();
    }
    else
    {
      stepper1.runSpeed();
    }
   }
 }
 else
  {
    digitalWrite(LI,LOW);//envia energia baja al pin 4(apaga)
  }
}
//****************************************************************************
//****************************************************************************
//****************************************************************************
void whileInicio(){
  inic=0;
  digitalWrite(INI,LOW); 
  while(inic==0)
  {
    inicio();
  } 
}
//****************************************************************************
//****************************************************************************
//****************************************************************************

//metodo que detemina la hora
void hotaconf(int a)
{
digitos++;
//aqui se configura la hora el primer digito de la hora, esta esta configurada en Americana osea 24 horas
if(digitos==1){
  if (a<3){ horas=a*10; }
} 
//aqui se configura el segundo valor que tendra la hora, si es 10 sera 11, 12, 13 ,ect, pero si es 20 solo se permitira 1,2,3, para que de 21,22,23.
if(digitos==2){

  if (horas==20&&a<4){ horas+=a; }
  
  if (horas<20){ horas+=a; }

} 
//aqui configuramos los minutos, para la hora, igual que antes es el primer valor de los minutos, si ponemos un 1, valdra 10, si pones 5 seran 50 minutos 
if(digitos==3){

  if(a<6){ minutos=a*10; }

} 
if(digitos==4){
  minutos+=a; // aqui se configura el ultimo digito de los minutos completando, que si presionaste anteriormente 5, y ahora presionas 8 el valor de los minutos final seria 58.
  digitos=0; // reiniciamos la variable digitos

  donde=0;// reiniciamos la bandera, y aqui decimos que ya no estamos configurando la hora

  lcd.setCursor(0, 0);
  lcd.print("Hora Configurada");

whileInicio();
  switch (horas) {
             case 8            : myStepper.step(-stepsPerRevolution); break;
             case 9            : myStepper.step(-stepsPerRevolution * 2);  break;
             case 10           : myStepper.step(-stepsPerRevolution * 3);  break;
             case 11           : myStepper.step(-stepsPerRevolution * 4);  break; 
             case 12           : myStepper.step(-stepsPerRevolution * 5); break;
             case 13           : myStepper.step(-stepsPerRevolution * 6); break;
             case 14           : myStepper.step(-stepsPerRevolution * 7); break;
             case 15           : myStepper.step(-stepsPerRevolution * 8);  break;
             case 16           : myStepper.step(-stepsPerRevolution * 9); break;
             case 17           : myStepper.step(-stepsPerRevolution * 10); break;
             case 18           : myStepper.step(-stepsPerRevolution * 11); break;
        }
        
        leonHora = horas;
        RGB(0x0000FF00);                                      // cambiamos el color del rgb de configuracion a verde.
        analogWrite(R_PIN,r);
        analogWrite(G_PIN,g);
        analogWrite(B_PIN,b);
} 

//prende y apaga el bombillo de ensendido, este deja saber si se preciono un boton del control para configurar la hora.
digitalWrite(LI,0);
delay(500);
digitalWrite(LI,1);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
//metodo que asigna el valor del color al led rgb
void RGB(unsigned long valor) {
   r = valor >> 16; 
   g = (valor >> 8) & 0xFF; 
   b = valor & 0xFF; 
}
//------------------------------------------------
//Metodo que mantiene un conteo de los segundos, minutos y horas
//transcuridos al llamar el metodo
//****************************************************************************
//*********************************HORA**************************************
//****************************************************************************
void hora()
{
  //funcion de calculo de milicegundos trascurridos
  unsigned long currentMillis = millis();
  //si los milisegundos trancurridos son 1000 suma un segundo
  if(currentMillis - previousMillis > 1000) 
  {
    segundos++;
   /*Permite saver el tiempo por monitor serial*/
   /*Serial.print("Hora ");
    Serial.print(horas);
    Serial.print(":");
    Serial.print(minutos);
    Serial.print(":");
    Serial.println(segundos);*/
    printHora();
    previousMillis = currentMillis;   
  }

// si los segundos trascurridos son 60 suma un minuto segundos es igual a 0
if(segundos==59){ segundos=0; minutos++; }
// si los minutos trascurridos son 60 suma una hora minutos es igual a 0
if(minutos==59){ minutos=0; horas++; }
// si las horas trascurridos son 24 hora es igual a 0
if(horas==23){ horas=0; }
}
//********************************INICIO**************************************
//****************************************************************************
//****************************************************************************
// Inicio es el metodo que establece donde se encuentra el inicio de donde debe partir el motor.
void inicio(){  
 //lee el pin 2 y determina si esta ensendido o pagado
 //pregunta si el estado ensendio y el estado anterior era apagado
  if(digitalRead(INI)==0 && inic==0)
  {
    stepper1.setSpeed(-100);
    stepper1.runSpeed();
  }
  else
  { 
    inic =1;
    stepper1.setSpeed(0); 
  }

}
//****************************************************************************
//****************************************************************************
//****************************************************************************
void determinarHora()
{
  if((leonHora<7||leonHora>19)&&inic==1)
  {
    digitalWrite(INI,LOW); 
    inic= 0; 
    leonHora=7;
  }

  switch (horas) 
  {
          //acciones        
             case 7            : if(leonHora==7){myStepper.step(-stepsPerRevolution);leonHora=8;} break;
             case 8            : if(leonHora==8){myStepper.step(-stepsPerRevolution);leonHora=9;} break;
             case 9            : if(leonHora==9){myStepper.step(-stepsPerRevolution);leonHora=10;}  break;
             case 10           : if(leonHora==10){myStepper.step(-stepsPerRevolution);leonHora=11;}  break;
             case 11           : if(leonHora==11){myStepper.step(-stepsPerRevolution);leonHora=12;}  break; 
             case 12           : if(leonHora==12){myStepper.step(-stepsPerRevolution);leonHora=13;} break;
             case 13           : if(leonHora==13){myStepper.step(-stepsPerRevolution);leonHora=14;} break;
             case 14           : if(leonHora==14){myStepper.step(-stepsPerRevolution);leonHora=15;} break;
             case 15           : if(leonHora==15){myStepper.step(-stepsPerRevolution);leonHora=16;}  break;
             case 16           : if(leonHora==16){myStepper.step(-stepsPerRevolution);leonHora=17;} break;
             case 17           : if(leonHora==17){myStepper.step(-stepsPerRevolution);leonHora=18;} break;
             case 18           : if(leonHora==18){myStepper.step(-stepsPerRevolution);leonHora=7;} break;
  }
}
//****************************************************************************
//****************************************************************************
//**************************************************************************** 

//este metodo imprime la hora en la pantalla LCD
void printHora()
 {
  if(horas<10)
  {
   lcd.setCursor(0, 1);
   lcd.print("0");
   lcd.setCursor(1, 1);
   lcd.print(horas);
  }
  else
  { 
    lcd.setCursor(0, 1);
    lcd.print(horas);
  }
   lcd.setCursor(2, 1);
    lcd.print(":");
  
  if(minutos<10)
  {
    lcd.setCursor(3, 1);
    lcd.print("0");
    lcd.setCursor(4, 1);
    lcd.print(minutos);
  }
  else
  { 
    lcd.setCursor(3, 1);
    lcd.print(minutos);
  }
 
   lcd.setCursor(5, 1);
   lcd.print(":");
   
  if(segundos<10)
  {
    lcd.setCursor(6, 1);
    lcd.print("0");
    lcd.setCursor(7, 1);
    lcd.print(segundos);
  }
  else
  { 
    lcd.setCursor(6, 1);
    lcd.print(segundos);
  }
  
 }
