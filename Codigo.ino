// Definição dos Pinos dos Botões
const int botoes[4] = {2, 3, 4, 5}; // TL, TR, BL, BR

// Definição dos Pinos dos LEDs
const int leds[4] = {8, 9, 10, 11}; // TL, TR, BL, BR

// Pino do Buzzer
const int buzzer = 12;

// Variáveis do Jogo
int blocoAtual = -1;
int pontuacao = 0;
unsigned long tempoLimite = 2000; // Tempo inicial que você tem para clicar (2 segundos)
unsigned long tempoInicioBloco = 0;

void setup() {
  // Inicializa os pinos
  for (int i = 0; i < 4; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
  }
  pinMode(buzzer, OUTPUT);
  
  // Semente para gerar números realmente aleatórios
  randomSeed(analogRead(A0));
  
  Serial.begin(9600);
  novoBloco();
}

void loop() {
  // 1. Verifica se o tempo para responder esgotou
  if (millis() - tempoInicioBloco > tempoLimite) {
    gameOver();
    return;
  }

  // 2. Verifica se algum botão foi pressionado
  for (int i = 0; i < 4; i++) {
    if (digitalRead(botoes[i]) == LOW) { // Botão pressionado (LOW por causa do PULLUP)
      
      if (i == blocoAtual) {
        // Acertou!
        acerto();
        novoBloco();
        delay(150); // Debounce para evitar leituras duplicadas
      } else {
        // Errou o botão!
        gameOver();
      }
      break; 
    }
  }
}

// Função para gerar o próximo "Tile" do jogo
void novoBloco() {
  // Apaga o LED anterior
  if (blocoAtual != -1) {
    digitalWrite(leds[blocoAtual], LOW);
  }
  
  // Sorteia um novo bloco (0 a 3) diferente do atual para não repetir seguidamente
  int sorteio;
  do {
    sorteio = random(0, 4);
  } while (sorteio == blocoAtual);
  
  blocoAtual = sorteio;
  
  // Acende o novo LED
  digitalWrite(leds[blocoAtual], HIGH);
  
  // Registra o momento em que o bloco acendeu
  tempoInicioBloco = millis();
  
  // Diminui o tempo limite sutilmente a cada ponto para aumentar a dificuldade
  tempoLimite = max(400UL, 2000 - (pontuacao * 50)); 
}

// Efeito sonoro e visual de acerto
void acerto() {
  pontuacao++;
  Serial.print("Pontuacao: ");
  Serial.println(pontuacao);
  
  // Som rápido de acerto (Nota Mi)
  tone(buzzer, 659, 80); 
}

// Animação e som de Game Over
void gameOver() {
  Serial.print("Fim de Jogo! Pontuacao Final: ");
  Serial.println(pontuacao);
  
  // Apaga o bloco ativo
  digitalWrite(leds[blocoAtual], LOW);
  
  // Som triste de Game Over (Frequência caindo)
  tone(buzzer, 300, 300);
  delay(150);
  tone(buzzer, 200, 400);
  delay(300);
  
  // Pisca todos os LEDs 3 vezes
  for (int piscada = 0; piscada < 3; piscada++) {
    for (int i = 0; i < 4; i++) digitalWrite(leds[i], HIGH);
    delay(200);
    for (int i = 0; i < 4; i++) digitalWrite(leds[i], LOW);
    delay(200);
  }
  
  // Reseta as variáveis do jogo para recomeçar
  pontuacao = 0;
  tempoLimite = 2000;
  blocoAtual = -1;
  
  delay(1000); // Espera 1 segundo antes de iniciar a próxima partida
  novoBloco();
}