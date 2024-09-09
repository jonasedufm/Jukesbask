// Definindo os pinos dos LEDs
const int ledPins[] = {2, 3, 4, 5}; // LEDs para contagem de 4 segundos
// Definindo o pino do botão de início
const int buttonPin = 7;
// Definindo o pino do controle da válvula solenoide
const int solenoidPin = 6; 
// Definindo os pinos dos displays de 7 segmentos (usaremos 2 displays)
const int displayPins[] = {8, 9, 10, 11, 12, 13, A0, A1}; // pinos dos segmentos dos displays
// Definindo o pino do sensor de contagem de pontos
const int sensorPin = A2;
// Definindo o pino do buzzer
const int buzzerPin = A3;

// Variáveis de controle
int pontos = 0;
bool jogoIniciado = false;
unsigned long tempoInicio;
unsigned long tempoJogo = 30000; // 30 segundos de jogo
unsigned long intervaloMovimento = 1000; // Tempo de intervalo para o movimento da cesta
unsigned long ultimoMovimento = 0;
bool direcaoCesta = false; // Falso para direita, verdadeiro para esquerda

void setup() {
  // Configurando os pinos
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(solenoidPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Configurando os displays
  for (int i = 0; i < 8; i++) {
    pinMode(displayPins[i], OUTPUT);
  }

  // Inicia com os displays desligados
  resetDisplay();
}

void loop() {
  // Verifica se o botão foi pressionado para iniciar o jogo
  if (digitalRead(buttonPin) == LOW && !jogoIniciado) {
    iniciarJogo();
  }

  // Se o jogo está em andamento
  if (jogoIniciado) {
    // Contagem do tempo do jogo
    if (millis() - tempoInicio < tempoJogo) {
      moverCesta();
      contarPontos();
      atualizarDisplay(pontos);
    } else {
      // Final do jogo
      pararJogo();
    }
  }
}

// Função para iniciar o jogo
void iniciarJogo() {
  // Contagem regressiva com os LEDs
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(1000);
    digitalWrite(ledPins[i], LOW);
  }

  // Inicia o movimento da cesta
  tempoInicio = millis();
  ultimoMovimento = millis();
  jogoIniciado = true;
}

// Função para mover a cesta
void moverCesta() {
  // Alterna a direção do cilindro a cada intervalo de tempo
  if (millis() - ultimoMovimento > intervaloMovimento) {
    // Alterna o estado da direção
    direcaoCesta = !direcaoCesta;
    
    // Aciona a válvula solenoide para mudar a direção do cilindro
    digitalWrite(solenoidPin, direcaoCesta ? HIGH : LOW);
    
    // Atualiza o tempo do último movimento
    ultimoMovimento = millis();
    
    // Reduz o intervalo de movimento para aumentar a dificuldade ao longo do tempo
    intervaloMovimento = max(200, intervaloMovimento - 50); // A cada ciclo, reduz o intervalo
  }
}

// Função para contar os pontos
void contarPontos() {
  // Verifica se a bola passou pelo sensor
  if (digitalRead(sensorPin) == LOW) {
    pontos++;
    delay(300); // Debounce do sensor
  }
}

// Função para atualizar o display de 7 segmentos
void atualizarDisplay(int pontos) {
  int unidade = pontos % 10;
  int dezena = (pontos / 10) % 10;
  mostrarNumero(unidade, displayPins); // Display da unidade
  mostrarNumero(dezena, displayPins); // Display da dezena
}

void mostrarNumero(int numero, const int *pinos) {
  // Segmentos a serem acesos para cada número (0 a 9)
  const bool segmentos[10][8] = {
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW}, // 0
    {LOW, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW},     // 1
    {HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH, LOW},  // 2
    {HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH, LOW},  // 3
    {LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW},   // 4
    {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH, LOW},  // 5
    {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}, // 6
    {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW},    // 7
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},// 8
    {HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH, LOW}  // 9
  };
  for (int i = 0; i < 8; i++) {
    digitalWrite(pinos[i], segmentos[numero][i]);
  }
}

// Função para parar o jogo e ativar o buzzer
void pararJogo() {
  jogoIniciado = false;
  digitalWrite(solenoidPin, LOW); // Para o movimento do cilindro
  digitalWrite(buzzerPin, HIGH); // Ativa o buzzer
  delay(2000); // Buzzer soa por 2 segundos
  digitalWrite(buzzerPin, LOW); // Desativa o buzzer
}

// Função para resetar os displays
void resetDisplay() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(displayPins[i], LOW);
  }
}
