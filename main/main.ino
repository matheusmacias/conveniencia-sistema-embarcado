#include <SPI.h>
#include <Ethernet.h>
#include <CustomJWT.h>

#include "WebServer.h"
#include "HeaderParser.h"

char key[] = "secret";

CustomJWT jwt(key, 256);

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress ip(10, 10, 81, 215);  // Endereço IP que a Ethernet Shield terá. Deve ser alterado para um endereço livre da sua rede.
int port(80);                   // Cria um servidor WEB

WebServer webServer(mac, ip, port);

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Inicializa o servidor web
  webServer.begin();

  Serial.println("Servidor web iniciado.");
}

void loop() {
  webServer.listen();
  if (webServer.getHeaders().length() > 0) {
    HeaderParser headerparser(webServer.getHeaders());
    String authorization = headerparser.getAuthorization(true);
    authorization.trim();
    if (authorization.length() > 0) {
      jwt.allocateJWTMemory();
      Serial.println(jwt.decodeJWT(authorization.c_str()));
      Serial.print("Header: ");
      Serial.print(jwt.header);
      Serial.print("\nHeader Length: ");
      Serial.println(jwt.headerLength);
      Serial.print("Payload: ");
      Serial.print(jwt.payload);
      Serial.print("\nPayload Length: ");
      Serial.println(jwt.payloadLength);
      Serial.print("Signature: ");
      Serial.print(jwt.signature);
      Serial.print("\nSignature Length: ");
      Serial.println(jwt.signatureLength);
      jwt.clear();
    }
  }
  webServer.resetHeaders();
}
