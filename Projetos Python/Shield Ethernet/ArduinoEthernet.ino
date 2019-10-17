#include <Keypad.h>
#include <Ethernet.h>
#include <SPI.h>

#define LED 13
#define RELE 11 //Porta digital 6 PWM

byte mac[] = { 0xBE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 20, 100 };
byte server[] = { 192, 168, 20, 2 }; // Touchberry Pi Server
int tcp_port = 65432;

EthernetClient client;

//Exemplo:
//http://wiring.org.co/reference/libraries/Keypad/Keypad_addEventListener_.html

const byte ROWS = 4; 
const byte COLS = 4; 

//Configuração de teclado da faculdade
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//Ordem dos pinos da faculdade
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Ethernet.begin(mac, ip);
  Serial.begin(9600);  

  pinMode(LED, OUTPUT); 
  pinMode(RELE, OUTPUT);   
  digitalWrite(RELE, HIGH); //Para começar com a porta fechada

  delay(1000);

  Serial.println("Connecting...");
  int conectado = 0;

  while (!conectado){
    if (client.connect(server, tcp_port)) { // Connection to server.js
      Serial.println("Connectado ao servidor");
      client.write(147);
      delay(1000);
      conectado = 1;
    }
    delay(200);
  }
}

int contador = 0;
char customKeyArray[10] = {};
char liberou = 0;

void loop(){

//Reiniciar o contador se passar do numero máximo
if (contador >=9){    
    contador = 0;
}

//Se a conexão serial estiver disponível
if (client.available()){
  char c = client.read();
  Serial.write(c);
  if (Serial.available()){
    if (liberou){  
        char serialListener = client.read(); 

        //Liberar caso receber um sinal 'S'    
        if (serialListener == 'S'){
            digitalWrite(LED, HIGH); //Acender LED
            digitalWrite(RELE, LOW); //Liberar porta
            delay(1000);
            digitalWrite(LED, LOW); //Apagar LED
            digitalWrite(RELE, HIGH); //Fechar porta
            liberou = 0;  //Impedir de entrar novamente nesse bloco até limpar o array          
        }
        //Exibir falha caso receber um sinal 'F'
        if (serialListener == 'F'){           
            liberou = 0;
        }      
    }
  }
}

if (!client.connected()) {
    Serial.println();
    Serial.println("Conexão perdida. Reconectando...");
    client.stop();
    int conectado = 0;
    
    while (!conectado){
      if (client.connect(server, tcp_port)) { // Connection to server.js
        Serial.println("Connectado ao servidor");        
        client.write(147);
        delay(1000);
        conectado = 1;  
      }
    delay(200);
  }
  }

  //Pegar a tecla digitada
  char customKey = customKeypad.getKey(); 
  //Adicionar somente digitos numéricos para o array
  if (customKey && isdigit(customKey)){
    customKeyArray[contador++] = customKey;
  }
 
  //Imprimir caso a tecla seja *
  //Somente enviar caso tenha sido digitado 1 numero
  if (customKey == '*' && contador > 0){

    //Finalizando a linha para enviar o array
    customKeyArray[contador] = '\0';
    Serial.println(customKeyArray);
    client.write(customKeyArray);

    //Resetando o array e o contador
    customKeyArray[0] = '\0';
    contador = 0;    
    liberou = 1; //Pode fazer a leitura do serial Listener    
  }

delay(100);
  
}