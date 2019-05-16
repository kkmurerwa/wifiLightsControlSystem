/*
 * Arduino code to control an item without internet connection
 * Author: Kenneth Murerwa
 */

//Import the required libraries
#include <WiFi.h>//import wifi module

//Declare the wifi settings for your router for connection
const char* ssid = "FabLab Winam";
const char* password = "TekTech42";

//Declare the port to be used by the WiFi server
WiFiServer server(80);

void setup() {
  Serial.begin(9600);//Set baud rate for the ESP32 WiFi transmission

  //Declare the pin ports
  pinMode(17, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(32, OUTPUT);
  delay(10);

  //Connect to the WiFi network
  Serial.println("Connecting");
  WiFi.begin(ssid, password);//Connect to the hotspot
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");  
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP()); //Print the ip address assigned to the wifi module
  
  server.begin();//Start the server

  int value = 0;
}

void loop() {
  WiFiClient clientt = server.available();//Listen for clientts and returns a boolean value if true
  if (clientt){//if clientt is found
    String currentLine = "";//String holds data from clientt
    while (clientt.connected()){//Executes if clientt is still connected
      if (clientt.available()){//If there is data from the clientt
        char c = clientt.read();
        Serial.write(c); 
        if (c == '\n'){
          if (currentLine.length() == 0){
            clientt.println("HTTP/1.1 200 OK");
            clientt.println("content-type: text/html");
            clientt.println();
            //Create the HTTP response
            clientt.println("<style>button{margin-left: 20px;margin-top: 20px; border-radius: 4px; height: 80px; width: 350px;font-size: 30px;}</style><div align='center'><a href = \'/A\'><button>TURN ON LED ONE</button></a><a href = \'/B\'><button>TURN OFF LED ONE</button></a> <br><a href = \'/C\'><button>TURN ON LED TWO</button></a><a href = \'/D\'><button>TURN OFF LED TWO</button></a> <br><a href = \'/E\'><button>TURN ON LED THREE</button></a><a href = \'/F\'><button>TURN OFF LED THREE</button></a> <br></div>");
            break;
          }
          else{
            currentLine = "";
          }
        }
        else if (c != '\r'){
            currentLine += c;
        }

        if (currentLine.endsWith("GET /A")){
          digitalWrite(17, HIGH);
        }
        if (currentLine.endsWith("GET /B")){
          digitalWrite(17, LOW);
        }
        if (currentLine.endsWith("GET /C")){
          digitalWrite(32, HIGH);
        }
        if (currentLine.endsWith("GET /D")){
          digitalWrite(32, LOW);
        }
        if (currentLine.endsWith("GET /E")){
          digitalWrite(27, HIGH);
        }
        if (currentLine.endsWith("GET /F")){
          digitalWrite(27, LOW);
        }
      }
    }
    //Close Connection
    clientt.stop();
  }
}
