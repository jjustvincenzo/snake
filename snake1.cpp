// snake1.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

const int larghezza = 20;
const int lunghezza = 30;

int x, y, fruitCordX, fruitCordY, playerscore, snakeTailX[100], snakeTailY[100];
int snakeTailLen;
enum snakesDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
snakesDirection sDir;
bool GameOver;

void GameInit() {
    GameOver = false;
    sDir = STOP;
    x = larghezza / 2;
    y = lunghezza / 2;
    fruitCordX = rand() % larghezza;
    fruitCordY = rand() % lunghezza;
    playerscore = 0;
    snakeTailLen = 0;
}

void GameRender(string playername) {
    // Disegno il campo nella console
    cout << "\033[H"; // ANSI escape code per tornare in cima alla console
    for (int i = 0; i < lunghezza + 2; i++) cout << "-";
    cout << endl;

    for (int i = 0; i < larghezza; i++) {
        for (int j = 0; j < lunghezza + 2; j++) {
            if (j == 0 || j == lunghezza + 1)
                cout << "|"; // Bordo laterale
            else if (i == y && j == x)
                cout << "O"; // Testa del serpente
            else if (i == fruitCordY && j == fruitCordX)
                cout << "F"; // Frutto
            else {
                bool printTail = false;
                for (int k = 0; k < snakeTailLen; k++) {
                    if (snakeTailX[k] == j && snakeTailY[k] == i) {
                        cout << "o"; // Corpo del serpente
                        printTail = true;
                    }
                }
                if (!printTail) cout << " ";
            }
        }
        cout << endl;
    }

    for (int i = 0; i < lunghezza + 2; i++) cout << "-";
    cout << endl;

    // Visualizzo il punteggio
    cout << playername << " Punteggio: " << playerscore << endl;
}

void UpdateGame(bool& campoCambiato) {
    // Memorizza la posizione precedente della testa del serpente
    int prevX = snakeTailX[0];
    int prevY = snakeTailY[0];
    int prev2X, prev2Y;

    // Aggiorna la posizione della coda
    snakeTailX[0] = x;
    snakeTailY[0] = y;

    for (int i = 1; i < snakeTailLen; i++) {
        prev2X = snakeTailX[i];
        prev2Y = snakeTailY[i];
        snakeTailX[i] = prevX;
        snakeTailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Aggiorna la posizione della testa in base alla direzione
    switch (sDir) {
    case LEFT:  x--; break;
    case RIGHT: x++; break;
    case UP:    y--; break;
    case DOWN:  y++; break;
    default:    break; // Nessun movimento se STOP
    }

    // Controllo collisione con i bordi
    if (x < 0 || x >= larghezza || y < 0 || y >= lunghezza) {
        GameOver = true;
        campoCambiato = true; // Campo modificato: gioco finito
        return;
    }

    // Controllo collisione con la coda
    for (int i = 0; i < snakeTailLen; i++) {
        if (snakeTailX[i] == x && snakeTailY[i] == y) {
            GameOver = true;
            campoCambiato = true; // Campo modificato: gioco finito
            return;
        }
    }

    // Controllo se il serpente mangia il frutto
    if (x == fruitCordX && y == fruitCordY) {
        playerscore += 10;
        fruitCordX = rand() % larghezza;
        fruitCordY = rand() % lunghezza;
        snakeTailLen++; // Aumenta la lunghezza della coda
        campoCambiato = true; // Campo modificato: frutto mangiato
    }

    // Verifica se la posizione è cambiata per necessità di aggiornamento
    if (x != snakeTailX[0] || y != snakeTailY[0]) {
        campoCambiato = true; // Campo modificato: serpente si è mosso
    }
}


int Setdifficulty() {
    int scelta;
    cout << "Scegli la difficolta (1 facile, 2 medio, 3 difficile, 4 estrema): ";
    cin >> scelta;

    switch (scelta) {
    case 1: return 300;
    case 2: return 200;
    case 3: return 100;
    case 4: return 50;
    default: return 200;
    }
}

void UserInput() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (sDir != RIGHT) sDir = LEFT;
            break;
        case 'd':
            if (sDir != LEFT) sDir = RIGHT;
            break;
        case 'w':
            if (sDir != DOWN) sDir = UP;
            break;
        case 's':
            if (sDir != UP) sDir = DOWN;
            break;
        case 'x':
            GameOver = true;
            break;
        }
    }
}

int main() {
    string playername;
    cout << "Inserire il nome del player: ";
    cin >> playername;

    int dfc = Setdifficulty();
    GameInit();

    // Setup console
    cout << "\033[2J"; // ANSI escape code per pulire lo schermo
    bool campoCambiato = true; // Inizialmente il campo deve essere disegnato

    while (!GameOver) {
        if (campoCambiato) {
            GameRender(playername); // Renderizza il campo solo se modificato
            campoCambiato = false;
        }
        UserInput();
        UpdateGame(campoCambiato); // Aggiorna lo stato del gioco e verifica modifiche
        Sleep(dfc);                // Pausa per controllare la velocità del gioco
    }
    cout << "Game Over, " << playername << "! Punteggio finale: " << playerscore << endl;
    return 0;
}
