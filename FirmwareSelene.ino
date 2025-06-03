String inputString = "";
bool stringComplete = false;
int analogPin = A0;  // Pino analógico padrão

void setup() {
  Serial.begin(9600);
  inputString.reserve(50);
}

void loop() {
  if (stringComplete) {
    processCommand(inputString);
    inputString = "";
    stringComplete = false;
  }

  delay(1000);
}

void processCommand(String command) {
  if (!command.startsWith("#")) return;

  String content = command.substring(1);  // Remove o #
  content.trim();

  if (content == "VALOR") {
    int value = analogRead(analogPin);
    Serial.print("#VALOR=");
    Serial.println(value);
  } else if (content == "LIGAR") {
    Serial.println("#LIGAR OK");
  } else if (content.startsWith("CANAL=")) {
    String canal = content.substring(7); // Pega o texto após "CANAL="
    canal.trim();

    if (isValidAnalogPin(canal)) {
      analogPin = analogStringToPin(canal);
      Serial.println("#CANAL OK");
    } else {
      Serial.println("#ERRO");
    }
  } else {
    Serial.println("#ERRO");
  }
}

// Verifica se o nome do canal analógico é válido (ex: A0 a A15)
bool isValidAnalogPin(String canal) {
  if (canal.length() < 2 || canal.charAt(0) != 'A') return false;
  int num = canal.substring(1).toInt();
  return num >= 0 && num <= 15;
}

// Converte "A0".."A15" em valor numérico de pino
int analogStringToPin(String canal) {
  int num = canal.substring(1).toInt();
  return A0 + num;
}

// Interrupção serial
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}
