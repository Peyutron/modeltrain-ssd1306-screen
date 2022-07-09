/* -------------------------------------------------------
//Pantalla SSD1306 Oled 128x64 conexion i2C
//Arduino UNO
//
//  OLED: SDA=A4, SLC=A5
//
//**Más proyectos, ejemplos y curiosidades en 
//  http://lamaquetade.infotronikblog.com y 
//  http://www.infotronikblog.com
*/
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

const int destinos = 7;   //Número de destinos

char relojPantalla[6];
char salida[7] = {"Salida"};
char compania[5] = {"Adif"};
char subtitulo[22] = "Hora  Llegadas    Via";

///Almacenamos el nombre de las estaciones
char destino[destinos][18] = { "Murcia del Carmen",
                               "Garrucha         ",
                               "Ameria Intermodal",
                               "Tardienta        ",
                               "M.Zambrano       ",
                               "S.Justa          ",
                               "Cordoba Central  " 
                              };
///Almacenamos el horario
char horario[destinos][6] = { "00:42",
                              "00:45",
                              "03:14",
                              "07:20",
                              "08:00",
                              "15:34",
                              "20:12"
                            };
///numero de vias
char vias [destinos][3] = { "02", "01", "02", "01", "02"};//, "01"};
int rotacion = 0;
int scrollPos = 0;
boolean scrolling = false;
boolean estado = false;
////Reloj/////////////////////
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 5; // 1000;  //the value is a number of milliseconds
byte segundo = 0;
int minuto = 0;
int hora = 0;

void setup() {
  
  randomSeed(analogRead(A0)); //Hacemos que random inicie en un numero generado por A0
  hora = random(0, 23);       //Al estar sin conectar unicamente recibiremos ruido.
  minuto = random(0, 59);
}

//// Bucle principal del programa
void loop() {
    u8g.firstPage();  
  do {
    principal();
  } while( u8g.nextPage() );
  Segundos(); //Toma un segundo como referencia  
  Reloj();    //Genera la hora para mostrar en el panel

}

void principal() {
        if ( (String(relojPantalla) == String(horario[rotacion])) && (estado)){
          estado = false; 
          scrolling = true;
          } else estado = true;
          
    if (rotacion >= destinos) rotacion = 0;
    u8g.setFont(u8g_font_6x12);
    u8g.drawStr( 0, 12, salida);
    u8g.drawStr( 50, 12, relojPantalla);
    u8g.drawStr( 105, 12, compania);
    u8g.drawBox(0,14,128,1);
    //////////////////////////
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr( 2, 24, subtitulo);
    u8g.drawBox(0,26,128,1);
    //////////////////////////
    u8g.setFont(u8g_font_04b_03);
    if (scrolling) llegadas(); //Si los horarios coinciden inicia el scroll
    else{
    u8g.drawStr( 2, 33, horario[rotacion]);
    u8g.drawStr( 37, 33, destino[rotacion]);
    u8g.drawStr( 118, 33, vias[rotacion]);
    }
    u8g.drawBox(0,34,128,1);
    //////////////////////////
    u8g.drawStr( 2, 41, horario[rotacion+1]);
    u8g.drawStr( 37, 41, destino[rotacion+1]);
    u8g.drawStr( 118, 41, vias[rotacion+1]);
    u8g.drawBox(0,42,128,1);
    //////////////////////////
    u8g.drawStr( 2, 49, horario[rotacion+2]);
    u8g.drawStr( 37, 49, destino[rotacion+2]);
    u8g.drawStr( 118, 49, vias[rotacion+2]);
    u8g.drawBox(0,50,128,1);
    //////////////////////////
    u8g.drawStr( 2, 57, horario[rotacion+3]);
    u8g.drawStr( 37, 57, destino[rotacion+3]);
    u8g.drawStr( 118, 57, vias[rotacion+3]);
    u8g.drawBox(0,58,128,1);


}
//Generamos un segundo a partir de millis (
void Segundos(){
   currentMillis = millis();  //obtener el "tiempo" actual (en realidad el número de milisegundos desde que se inició el programa)
  if (currentMillis - startMillis >= period)  //Comprueba si el periodo a transcurrido
  {
    segundo++;
  }
}
//// Gerera la hora para poner el reloj
void Reloj(){
  delay(period);
  String minutos;
  String horas;
  if (segundo >= 59 ) segundo = 0;
  if (segundo == 0) minuto++;
  if (minuto >= 59) minuto = 0;
  if (minuto == 59) hora++;
  if (hora >= 23) hora = 0;
  if (minuto <= 9) minutos = ("0" + String(minuto));
  else minutos = String(minuto);
  if (hora <= 9) horas = ("0" + String(hora));
  else horas = String(hora);
  String horaCompleta = (horas + ":" + minutos);  //Carga el String con la hora 
  horaCompleta.toCharArray(relojPantalla, 6);     //en una variable tipo char
}
////Animación rotación del texto de izquierda a derecha
void llegadas(){
          if(scrollPos > -448){ // posicion horizontal scroll 
 scrollPos--; 
 } else{ scrollPos = 0;
        scrolling = false;
        delay (2000);
        rotacion++;
        } // cuando esta fuera de pantalla se inicia
    u8g.drawStr(scrollPos + 2, 33, horario[rotacion]);
    u8g.drawStr(scrollPos + 37, 33, destino[rotacion]);
    u8g.drawStr(scrollPos + 118, 33, vias[rotacion]);
   }



