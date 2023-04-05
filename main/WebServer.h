#include <SPI.h>
#include <Ethernet.h>

class WebServer {
  private:
    byte* mac;
    IPAddress ip;
    EthernetServer server;

    String headers;
    String authorization;
    bool currentLineIsBlank;

    void sendHeaders() {
      // envia o cabeçalho de uma resposta http padrão
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
    }

    void sendWebPage() {
      // ENVIA A PÁGINA WEB
      client.println("<!DOCTYPE html>");
      client.println("<html>");
      client.println("<head>");
      client.println("<title>Página do Arduino</title>");
      client.println("<meta charset=\"UTF-8\"/>");
      client.println("</head>");
      client.println("<body>");
      client.println("<h1>Servidor Funcionando!</h1>");
      client.println("<p>Loja autônoma em funcionamento!</p>");
      client.println("</body>");
      client.println("</html>");
    }

  public:
    EthernetClient client;

    WebServer(byte* macAddress, IPAddress ipAddress, int port) :
      mac(macAddress),
      ip(ipAddress),
      server(port) {}

    void begin() {
      Serial.begin(115200);
      Ethernet.begin(mac, ip);  // Inicializa a Ethernet Shield
      server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
    }

    String getHeaders(){
      return this->headers;
    }

    void resetHeaders(){
      headers = "";
    }
    

    void listen() {
      client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)

      if (client) {  // Existe um cliente em conexão ?
        currentLineIsBlank = true;
        while (client.connected()) {
          if (client.available()) {   // os dados do cliente estão disponiveis para serem lidos
            char c = client.read(); // lê 1 byte (character) do cliente
            headers += c;
            // a ultima linha da requisição do cliente é branca e termina com o caractere \n
            // responde para o cliente apenas após a última linha recebida
            if (c == '\n' && currentLineIsBlank) {
              sendHeaders();
              sendWebPage();
              break;
            }
            // toda linha de texto recebida do cliente termina com os caracteres \r\n
            if (c == '\n') {
              // ultimo caractere da linha do texto recebido
              // iniciando nova linha com o novo caractere lido
              currentLineIsBlank = true;
            } 
            else if (c != '\r') {
              // um caractere de texto foi recebido do cliente
              currentLineIsBlank = false;
            }
          } // fim do if (client.available())
        } // fim do while (client.connected())

        delay(1);      // da um tempo para o WEB Browser receber o texto
        client.stop(); // termina a conexão
      } // fim do if (client)
    } // fim do loop
};
