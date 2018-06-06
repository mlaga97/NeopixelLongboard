#include "LongboardNeopixel.h"

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(144, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(144, 4, NEO_GRB + NEO_KHZ800);

volatile uint32_t ticks = 0;
volatile uint32_t lastElapsed = 0;
volatile uint32_t lastMillis = 0;

// 0 = none, 1 = rgb_solid, 2 = rainbow_rails
int pattern = 2;
bool front_lights = false;
bool rear_lights = false;
bool direction = true;

// RGB Solid
int spacing = 5;
uint8_t rgb_rVal = 255;
uint8_t rgb_gVal = 255;
uint8_t rgb_bVal = 255;

void showColorParser(String input) {
	Serial.println(input);
	
	switch(input.charAt(0)) {
		case 'r':
			rgb_rVal = input.substring(2).toInt();
			break;
		case 'g':
			rgb_gVal = input.substring(2).toInt();
			break;
		case 'b':
			rgb_bVal = input.substring(2).toInt();
			break;
		case 's':
			spacing = input.substring(2).toInt();
			break;
	}
}



/**********************************************************************/

WiFiServer server = WiFiServer(80);

String prepareHtmlPage() {
  String htmlPage = 
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
            "\r\n" +
            "<html>\r\n\t<head>\r\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\t\t<link rel=\"stylesheet\" type=\"text/css\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\"></link>\r\n\t\t<style>\r\n\t\t\tbody {\r\n\t\t\t\twidth: 100%;\r\n\t\t\t\tcursor:'pointer';\r\n\t\t\t}\r\n\t\t</style>\r\n\t\t<script src=\"https://code.jquery.com/jquery-3.2.1.min.js\" integrity=\"sha256-hwg4gsxgFZhOsEEamdOYGBf13FyQuiTwlAQgxVSNgt4=\" crossorigin=\"anonymous\"></script>\r\n\t\t<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>\r\n\t\t<script>\r\n\t\t\tfunction httpGetAsync(theUrl, callback) {\r\n\t\t\t\tvar xmlHttp = new XMLHttpRequest();\r\n\t\t\t\txmlHttp.onreadystatechange = function() { \r\n\t\t\t\t\tif (xmlHttp.readyState == 4 && xmlHttp.status == 200)\r\n\t\t\t\t\t\tcallback(xmlHttp.responseText);\r\n\t\t\t\t}\r\n\t\t\t\txmlHttp.open(\"GET\", theUrl, true); // true for asynchronous \r\n\t\t\t\txmlHttp.send(null);\r\n\t\t\t}\r\n\t\t\t\r\n\t\t\tfunction setColor() {\r\n\t\t\t\tcolor = document.getElementById('rgb_color').value;\r\n\t\t\t\t\r\n\t\t\t\tif(color.charAt(0) == \"#\") {\r\n\t\t\t\t\tcolor = color.substring(1,7);\r\n\t\t\t\t}\r\n\t\t\t\t\r\n\t\t\t\thttpGetAsync('/showColor?r=' + parseInt(color.substring(0,2), 16) + '&g=' + parseInt(color.substring(2,4), 16) + '&b=' + parseInt(color.substring(4,6), 16), function() {});\r\n\t\t\t}\r\n\t\t</script>\r\n\t</head>\r\n\t<body>\r\n\t\t<nav class=\"navbar navbar-inverse bg-inverse\">\r\n\t\t\t<a class=\"navbar-brand\" href=\"#\">Neopixel Longboard Control Panel</a>\r\n\t\t</nav>\r\n\t\t\r\n\t\t<center>\r\n\t\t\t<form action=\"\">\t\t\t\t\r\n\t\t\t\t<h3>Options</h3>\r\n\t\t\t\t<div class=\"btn-group\" data-toggle=\"buttons\">\r\n\t\t\t\t\t<input type=\"button\" name=\"options\" class=\"btn btn-primary\" onclick=\"httpGetAsync('/toggleFront', function() {})\" value=\"Front Lights\"/>\r\n\t\t\t\t\t<input type=\"button\" name=\"options\" class=\"btn btn-primary\" onclick=\"httpGetAsync('/toggleRear', function() {})\" value=\"Rear Lights\"/>\r\n\t\t\t\t\t<input type=\"button\" name=\"options\" class=\"btn btn-primary\" onclick=\"httpGetAsync('/toggleDirection', function() {})\" value=\"Flip Direction\"/>\r\n\t\t\t\t</div>\r\n\t\t\t\t\r\n\t\t\t\t<h3>Select Pattern</h3>\r\n\t\t\t\t<div class=\"btn-group\" data-toggle=\"buttons\">\r\n\t\t\t\t\t<label class=\"btn btn-primary\" onclick=\"httpGetAsync('/showNone', function() {})\">\r\n\t\t\t\t\t\t<input type=\"radio\" name=\"pattern\" value=\"none\"> None<br>\r\n\t\t\t\t\t</label>\r\n\t\t\t\t\t<label class=\"btn btn-primary\" onclick=\"httpGetAsync('/showPattern?pattern=rainbow_rails', function() {})\">\r\n\t\t\t\t\t\t<input type=\"radio\" name=\"pattern\" value=\"rainbowRails\"> Rainbow Rails<br/>\r\n\t\t\t\t\t</label>\r\n\t\t\t\t\t<label class=\"btn btn-primary\" onclick=\"setColor()\">\r\n\t\t\t\t\t\t<input id=\"rgb_radio\" type=\"radio\" name=\"pattern\" value=\"RGB\"> RGB<br>\r\n\t\t\t\t\t</label>\r\n\t\t\t\t</div>\r\n\t\t\t\t\r\n\t\t\t\t<h3>Pick Color</h3>\r\n\t\t\t\t<input id=\"rgb_color\" type=\"color\" id=\"html5colorpicker\" value=\"#ffffff\" style=\"width:200px; height:200px;\" onchange=\"if(document.getElementById('rgb_radio').checked) {setColor();}\">\r\n\t\t\t</form>\r\n\t\t</center>\r\n\t</body>\r\n</html>\r\n";
  return htmlPage;
}

String requestLine;
String page;
String parameters;
String parameter;
String value;

void WifiStuff() {
	WiFiClient client = server.available();
	
	// wait for a client (web browser) to connect
	if (client)	{
		Serial.println("\n[Client connected]");

		while (client.connected())	{
			if (client.available())	{
				String requestLine = client.readStringUntil('\r');
				
				if(requestLine.substring(0,3) == String("GET")) {
					requestLine = requestLine.substring(4);
					requestLine = requestLine.substring(0, requestLine.indexOf(' '));
					
					// Get Page Name
					page = requestLine.substring(1, requestLine.indexOf('?'));
					
					// Get Parameters
					if(requestLine.indexOf('?') != -1) {
						parameters = requestLine.substring(requestLine.indexOf('?')+1);
					} else {
						parameters = "";
					}
					
					// Switch Case
					if(page == String("showColor")) {
						pattern = 1;
						
						while(parameters.indexOf('&') != -1) {	
							parameter = parameters.substring(0, parameters.indexOf('&'));
							parameters = parameters.substring(parameters.indexOf('&')+1);
							showColorParser(parameter);
						}
						if(parameters.length() > 0) {
							showColorParser(parameters);
						}
						
					} else if(page == String("showPattern")) {
						if(parameters == String("pattern=rainbow_rails")) {
							pattern = 2;
						}
					} else if(page == String("showNone")) {
						pattern = 0;
					} else if(page == String("toggleFront")) {
						front_lights = !front_lights;
					} else if(page == String("toggleRear")) {
						rear_lights = !rear_lights;
					} else if(page == String("toggleDirection")) {
						direction = !direction;
					}
				}
				
				Serial.print(requestLine);
				// wait for end of client's request, that is marked with an empty line
				if (requestLine.length() == 1 && requestLine[0] == '\n')	{					
					client.println(prepareHtmlPage());
					break;
				}
			}
		}
		delay(1); // give the web browser time to receive the data

		// close the connection:
		client.stop();
		Serial.println("[Client disonnected]");
	}
}

/**********************************************************************/

void hallEffectISR() {
  ticks++;
  lastElapsed = millis() - lastMillis;
  lastMillis = millis();
}

/**********************************************************************/

void updateGyro() {
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,6,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

/**********************************************************************/

void setup() {
  Serial.begin(115200);

  // MPU-6050
  Wire.begin(12, 14);
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  // Hall Effect Sensor
  pinMode(13, INPUT);
  digitalWrite(13, HIGH);
  attachInterrupt(13, hallEffectISR, FALLING);

  // LED Strips
  strip1.begin();
  strip1.show();
  
  strip2.begin();
  strip2.show();
  
  // Server Stuff
  WiFi.mode(WIFI_STA);
  WiFi.begin("WIFI_SSID", "WIFI_PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  
  server.begin();
}

/**********************************************************************/

void loop() {
	WifiStuff();

	renderSides(pattern);
	if(front_lights)
		renderFront();
	if(rear_lights)
		renderRear();

	strip1.show();
	strip2.show();
}
