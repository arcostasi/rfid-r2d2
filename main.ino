// Controle de acesso R2D2 com leitor RFID

// MFRC522 -> Pianagem do Arduino UNO
// RST -> 9
// SDA(SS) -> 10
// MOSI -> 11
// MISO -> 12
// SCK -> 13

// MFRC522 -> Pianagem do Arduino Mega
// RST -> 5
// SDA(SS) -> 53
// MOSI -> 51
// MISO -> 50
// SCK -> 52

#include <SPI.h>
#include "MFRC522.h"
#include "notes.h"

// Define os pinos SDA e RESET
#define SS_PIN 53
#define RST_PIN 5

// Pino onde o positivo do buzzer será ligado
const int speakerPin = 6;

// Cria instâcia do RFID-RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Vetor com as tags autorizadas          -> F6 0F 04 56
String tagsCadastrada[2] = { "49 6F 46 D5", "F6 0F 04 57" };

// Pinos do LED RGB
int ledVermelho = 2;
int ledVerde = 3;
int ledAzul = 4;

// Pino do relé
int relay = 7;

void setup()
{
    // Inicia a porta serial
    Serial.begin(9600);

    // Inicia a conexão SPI
    SPI.begin();

    // Inicia MFRC522
    mfrc522.PCD_Init();

    Serial.println();

    pinMode(speakerPin, OUTPUT);

    // Define os pinos do LED RGB como saída
    pinMode(ledVermelho, OUTPUT);
    pinMode(ledVerde, OUTPUT);
    pinMode(ledAzul, OUTPUT);

    // Método inicial
    mensagemLeitura();

    // Define o pino do relé como saída e coloca em nível alto
    pinMode(relay, OUTPUT);
    digitalWrite(relay, HIGH);
}

void loop()
{
    // Aguarda a presença do cartão / tag
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Verifica a leitura do cartão / tag
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // Mostra UID da tag na serial
    Serial.print("UID da tag:");

    // Variável que armazena o UID do cartão
    String tagUID = "";

    // Percorre byte a byte os dados lidos do cartão
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        // Concatena na variável tagUID
        tagUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        tagUID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    // Adiciona uma quebra de linha
    Serial.println();

    boolean tagVerificada = false;

    // Imprime o valor da variavel tagUID em Maisculo
    tagUID.toUpperCase();

    // Verifica se a tag lida coincide com uma das tags cadastada no array
    for (int i = 0; i < sizeof(tagsCadastrada); i++) {
        // Verifica se a tag está cadastrada
        if (tagUID.substring(1) == tagsCadastrada[i]) // UID 1
        {
            // Tag encontrada
            tagVerificada = true;
            Serial.println("Acesso liberado!");

            // Acende o led verde e liga o relé
            tagValida();

            // Tones de acesso liberado
            toneValido();

            // Inicia o processo novamente
            mensagemLeitura();

            // Limpa a String tagUID para fazer nova leitura
            tagUID = "";
            break;
        }
    }

    // Se a tagUID não foi verificada, informa ao usuário que não tem acesso
    if ((tagVerificada == false) && (tagUID != "")) {
        // Tag não encontrada
        Serial.println("Acesso negado!");

        // Acende o led vermelho e matem o relé desligado
        tagInvalida();

        // Tone de inválido
        toneInvalido();

        // Inicia o processo novamente
        mensagemLeitura();

        // Limpa a String tagUID para fazer nova leitura
        tagUID = "";
    }
}

// Método que informa a mensagem inicial
void mensagemLeitura()
{
    Serial.println();
    Serial.println("Aproxime o cartao do leitor");

    digitalWrite(ledAzul, HIGH);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, LOW);

    digitalWrite(relay, HIGH);
    Serial.println();
}

// Método chamado quando a tag for encontrada
void tagValida()
{
    digitalWrite(ledAzul, LOW);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);

    digitalWrite(relay, LOW);
}

// Método chamado quando a tag não for encontrada
void tagInvalida()
{
    digitalWrite(ledAzul, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);

    digitalWrite(relay, HIGH);
}

// Tones
void toneValido()
{
    for (int i = 100; i < 5000; i = i * 1.45) {
        newTone(speakerPin, i, 60);
    }
    delay(10);
    for (int i = 100; i < 6000; i = i * 1.5) {
        newTone(speakerPin, i, 20);
    }
    for (int i = 1000; i < 2000; i = i * 1.02) {
        newTone(speakerPin, i, 10);
    }
    for (int i = 2000; i > 1000; i = i * 0.98) {
        newTone(speakerPin, i, 10);
    }
    noTone(speakerPin);
}

void toneInvalido() {
    for (int i = 1108; i < 1244; i = i * 1.01) {
        newTone(speakerPin, i, 30);
    }
    delay(100);
    for (int i = 1244; i > 1158; i = i * 0.99) {
        newTone(speakerPin, i, 30);
    }
    noTone(speakerPin);
    delay(100);
}
