#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// Definicoes
#define BOARD_SIZE 3
#define EMPTY 0
#define PLAYER_X 1
#define PLAYER_O 2

// Estrutura do jogo
typedef struct {
    int board[BOARD_SIZE][BOARD_SIZE];
    int currentPlayer;
    int movesCount;
} Game;

// Funcoes da logica
void initGame(Game *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j] = EMPTY;
        }
    }
    game->currentPlayer = PLAYER_X;
    game->movesCount = 0;
}

void displayBoard(const Game *game) {
    printf("\n");
    printf("     0   1   2\n");
    printf("   +---+---+---+\n");
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf(" %d |", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            char cell;
            if (game->board[i][j] == EMPTY) {
                cell = ' ';
            } else if (game->board[i][j] == PLAYER_X) {
                cell = 'X';
            } else {
                cell = 'O';
            }
            printf(" %c |", cell);
        }
        printf("\n");
        printf("   +---+---+---+\n");
    }
    printf("\n");
}

bool isValidMove(const Game *game, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return false;
    }
    return game->board[row][col] == EMPTY;
}

bool makeMove(Game *game, int row, int col) {
    if (!isValidMove(game, row, col)) {
        return false;
    }
    
    game->board[row][col] = game->currentPlayer;
    game->movesCount++;
    return true;
}

char getPlayerChar(int player) {
    return (player == PLAYER_X) ? 'X' : 'O';
}

void switchPlayer(Game *game) {
    game->currentPlayer = (game->currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
}

// Verifica linhas
static bool checkRows(const Game *game, int player) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        bool win = true;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] != player) {
                win = false;
                break;
            }
        }
        if (win) return true;
    }
    return false;
}

// Verifica colunas
static bool checkCols(const Game *game, int player) {
    for (int j = 0; j < BOARD_SIZE; j++) {
        bool win = true;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (game->board[i][j] != player) {
                win = false;
                break;
            }
        }
        if (win) return true;
    }
    return false;
}

// Verifica diagonais
static bool checkDiagonals(const Game *game, int player) {
    // Diagonal principal
    bool diag1 = true;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (game->board[i][i] != player) {
            diag1 = false;
            break;
        }
    }
    
    // Diagonal secundaria
    bool diag2 = true;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (game->board[i][BOARD_SIZE - 1 - i] != player) {
            diag2 = false;
            break;
        }
    }
    
    return diag1 || diag2;
}

int checkWinner(const Game *game) {
    if (checkRows(game, PLAYER_X) || checkCols(game, PLAYER_X) || checkDiagonals(game, PLAYER_X)) {
        return PLAYER_X;
    }
    
    if (checkRows(game, PLAYER_O) || checkCols(game, PLAYER_O) || checkDiagonals(game, PLAYER_O)) {
        return PLAYER_O;
    }
    
    return 0;
}

bool isBoardFull(const Game *game) {
    return game->movesCount >= (BOARD_SIZE * BOARD_SIZE);
}

// Funcoes da interface
void printInstructions(void) {
    printf("\nJOGO DA VELHA\n");
    printf("Digite linha e coluna 0-2 para jogar\n");
    printf("X comeca primeiro\n\n");
}

bool parseInput(const char *input, int *row, int *col) {
    if (sscanf(input, "%d %d", row, col) != 2) {
        return false;
    }
    return true;
}

int main(void) {
    Game game;
    int row, col;
    char input[10];
    int winner;
    
    initGame(&game);
    printInstructions();
    
    while (1) {
        displayBoard(&game);
        
        winner = checkWinner(&game);
        if (winner != 0) {
            printf("\nParabens jogador %c ganhou\n", getPlayerChar(winner));
            printf("Pressione Enter para continuar\n");
            getchar();
            break;
        }
        
        if (isBoardFull(&game)) {
            printf("\nEmpate\n\n");
            break;
        }
        
        printf("Jogador %c: ", getPlayerChar(game.currentPlayer));
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Erro ao ler entrada\n");
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (!parseInput(input, &row, &col)) {
            printf("Entrada invalida\n\n");
            continue;
        }
        
        if (!makeMove(&game, row, col)) {
            printf("Posicao invalida ou ja ocupada\n\n");
            continue;
        }
        
        printf("\n");
        switchPlayer(&game);
    }
    
    printf("Jogar novamente - (s/n): ");
    fflush(stdout);
    
    if (fgets(input, sizeof(input), stdin) != NULL && tolower(input[0]) == 's') {
        system("cls");
        main();
    }
    
    printf("Ate logo\n");
    return 0;
}
