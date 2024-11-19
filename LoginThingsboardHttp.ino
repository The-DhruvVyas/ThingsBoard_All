/*
This is a example to login into thingsboard(iot platform ) to fetch the latest jwt token 
work on http post request 
compatible with arduino ide
support board are :
esp32,arduino328p,adafruit feather , raspberry pi pico 2040
*/
void login() {
  int L = 0;
  String temp = "";
  //login to thingsboard to fetch the new jwt token
  const char* apiEndpoint = "/api/auth/login";
  String line = "";
  // Connect to the server and make the request
  if (client.connect(server, port)) { // thimhsboard.cloud , 80/443
    Serial.println("Connected to server for login");

    // Construct JSON payload using DynamicJsonDocument
    DynamicJsonDocument doc(1024);
    doc["username"] = "yourusername@gmail.com";
    doc["password"] = "your password ";

    // Serialize JSON document to char array
    char json_data[256];  // Make sure this is large enough for your JSON data
    serializeJson(doc, json_data, sizeof(json_data));

    // Send HTTP POST request
    client.println("POST " + String(apiEndpoint) + " HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Accept: application/json");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(strlen(json_data));
    client.println();
    client.println(json_data);  // Send JSON payload

        // Wait for a response
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        //Serial.println(line);  // Print each line of the response
        L = L + 1;
        if (L == 1) {
          Serial.println(line);
        }
         if (L == 14) {
           temp = line;
           Serial.println(temp);  // Print each line of the response
        //   parseConfirm(temp);
        client.stop(); 
         break;
         }
        Serial.println(L);
        Serial.println(line);
      }
    }
    client.stop();  // Close the connection
    Serial.println("Disconnected from server");
  } else {
    Serial.println("Connection to server failed");
  }
  // Define DynamicJsonDocument size based on expected JSON content size
  DynamicJsonDocument doc1(2048);

  // Parse the JSON response
  DeserializationError error = deserializeJson(doc1, temp);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Extract the refreshToken from the JSON
  // String refreshToken = doc1["refreshToken"].as<String>();
  String refreshToken = doc1["token"].as<String>();
  

  // Print refreshToken to Serial
  Serial.print("Refresh Token: ");
  Serial.println(refreshToken);
  token = "Bearer " + refreshToken ;
  Serial.print("token: ");
  Serial.println(token);
}