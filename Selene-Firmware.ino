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
  int value = analogRead(A0);
  Serial.print("#DIST=");
  Serial.println(value);
  delay(500);
  value = analogRead(A1);
  Serial.print("#ANG=");
  Serial.println(value);
  delay(500);
  value = analogRead(A2);
  Serial.print("#INTN=");
  Serial.println(value);
  delay(500);
  value = analogRead(A3);
  Serial.print("#FORMA=");
  Serial.println(value);
  delay(500);
}

void processCommand(String command) {
  if (!command.startsWith("#")) return;

  String content = command.substring(1);  // Remove o #
  content.trim();

  if (content == "VALOR") {
    int value = analogRead(analogPin);
    Serial.print("#VALOR=");
    Serial.println(value);
  } else if (content == "AVANCAR") {
    Serial.println("#LIGAR OK");
  } else if (content == "RECUAR") {
    Serial.println("#LIGAR REVERSO OK");
  } else if (content == "DIREITA") {
    Serial.println("#GIRANDO ANTI-HORARIO OK");
  } else if (content == "ESQUERDA") {
    Serial.println("#GIRANDO ANTI-HORARIO OK");
  } else if (content == "PEGAR") {
    Serial.println("#PEGAR OK");
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
