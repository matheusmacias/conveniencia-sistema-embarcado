#include <SPI.h>
#include <Ethernet.h>
#include <CustomJWT.h>
#include <ArduinoJson.h>

#include "WebServer.h"
#include "HeaderParser.h"

/* -------------- CONFIG JWT -------------- */
char key[] = "secret";
CustomJWT jwt(key, 256);

/* ----------- CONFIG SERVIDOR ------------ */
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress ip(10, 10, 81, 215);  // Endereço IP que a Ethernet Shield terá. Deve ser alterado para um endereço livre da sua rede.
int port(80);                   // Cria um servidor WEB

WebServer webServer(mac, ip, port);

/* ----------------- JSON ----------------- */
StaticJsonDocument<200> doc;

/* ---------- PROTÓTIPOS FUNÇÕES ---------- */
void handleHeaders();

void setup() {
  Serial.begin(115200);

  webServer.begin();

  Serial.println("Servidor está sendo iniciado.");
}

void loop() {
  webServer.listen(handleHeaders);
}

void handleHeaders(){
  if (webServer.getHeaders().length() > 0) {
    HeaderParser headerparser(webServer.getHeaders());
    String authorization = headerparser.getAuthorization(true);
    authorization.trim();
    if (authorization.length() > 0) {
      jwt.allocateJWTMemory();
      Serial.println(jwt.decodeJWT(authorization.c_str()));

      DeserializationError error = deserializeJson(doc, jwt.payload);
      // if (error) {
      //   Serial.print(F("deserializeJson() failed: "));
      //   Serial.println(error.f_str());
      // }
      const char* name = doc["name"];
      if(name){
        Serial.println("Abrir Porta!");
        webServer.sendHeaders_OK();
      }else{
        Serial.println("Acesso Negado!");
        webServer.sendHeaders_Forbidden();
      }
      
      jwt.clear();
    }
  }
  webServer.resetHeaders();
}
