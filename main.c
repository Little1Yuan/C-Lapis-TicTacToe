#include <stdio.h>
#include <windows.h>

#define X 'X'
#define O 'O'
#define E ' ' // 空

/* 棋盘：
 * 0 | 1 | 2
 * --+---+--
 * 3 | 4 | 5
 * --+---+--
 * 6 | 7 | 8
 */
char board[9];
int turns; // 回合数

typedef enum Ending {
    PLAYER, COMPUTER, NONE, DRAW
} Ending;

void initGame(void) {
    for (int i = 0; i < 9; i++) {
        board[i] = E;
    }
    turns = 0;
}

void printBoardSplitLine(void) {
    printf("+---+---+---+\n"); // 分割线
}

void printBoardRow(const int row) {
    const int startIndex = row * 3; // row = 0, 1, 2
    printf("| %c | %c | %c |\n", board[startIndex + 0], board[startIndex + 1], board[startIndex + 2]);
}

void printTutorialBoardRow(const int row) {
    const int startIndex = row * 3 + 1; // row = 0, 1, 2
    printf("| %d | %d | %d |\n", startIndex, startIndex + 1, startIndex + 2);
}

void printBoard(void) {
    for (int i = 0; i < 3; i++) {
        printBoardSplitLine();
        printBoardRow(i);
    }
    printBoardSplitLine();
}

void printTutorial(void) {
    printf("-- Game Tutorial --\n");
    printf("Objective:\n");
    printf("Be the first player to get three of your marks in a row-horizontally, vertically, or diagonally.\n");
    printf("Two players: you use 'X', the computer uses 'O'.\n");
    printf("Game board:\n");
    for (int i = 0; i < 3; i++) {
        printBoardSplitLine();
        printTutorialBoardRow(i);
    }
    printBoardSplitLine();
    printf("You should take turns placing marks in empty squares by inputting the number of the slot you want.\n");
    printf("The first player to form a straight line of three of their marks wins.\n");
    printf("If all nine squares are filled and no player has three in a row, the game ends in a tie.\n");
    printf("Enjoy the game~\n");
}

int askForPlayAgain(const int fromErr) {
    printf(fromErr != 0 ? "Unknown operation %c! Please input 1 or 2.\n" : "Would you like to play again?\n", fromErr);
    printf("1 - Yes\n");
    printf("2 - No\n");
    printf("Enter your choice before playing again: ");
    int choice = '\n';
    while (choice == '\n') choice = getchar();
    switch (choice) {
        case '1':
            return 1;
        case '2':
            return 0;
        default:
            return askForPlayAgain(choice); // 输入不合法，重新询问
    }
}

int hasWin(char chess) {
    // 横向判定
    for (int i = 0; i < 3; i++) {
        int won = 1;
        for (int j = 0; j < 3; j++) {
            if (board[i * 3 + j] != chess) {
                won = 0;
                break;
            }
        }
        if (won) return 1;
    }
    // 纵向判定
    for (int i = 0; i < 3; i++) {
        int won = 1;
        for (int j = 0; j < 3; j++) {
            if (board[j * 3 + i] != chess) {
                won = 0;
                break;
            }
        }
        if (won) return 1;
    }
    // 斜向判定(\)
    int won = 1;
    for (int i = 0; i < 3; i++) {
        if (board[i * 3 + i] != chess) {
            won = 0;
            break;
        }
    }
    if (won) return 1;
    // 斜向判定(/)
    won = 1;
    for (int i = 0; i < 3; i++) {
        if (board[i * 3 + 2 - i] != chess) {
            won = 0;
            break;
        }
    }
    if (won) return 1;
    return 0;
}

Ending winnerJudge(void) {
    if (hasWin('X')) return PLAYER;
    if (hasWin('O')) return COMPUTER;
    for (int i = 0; i < 9; i++) {
        if (board[i] != E) {
            return NONE;
        }
    }
    // 格子全部被占满即为平局
    return DRAW;
}

void playerAction(void) {
    printf("Please enter the square you want (1-9): ");
    while (1) {
        const int p = getchar();
        if (p == '\n') continue;
        if (p < '1' || p > '9') {
            printf("Illegal square number! Please enter again (1-9): ");
            continue;
        }
        const int place = p - '0' - 1;
        if (board[place] != E) {
            printf("This square is not empty! Please enter another number (1-9): ");
            continue;
        }
        board[place] = X;
        return;
    }
}

int computerDecision(void) {
    // 穿心三连
    if (board[4] == O) {
        for (int i = 0; i < 9; i++) {
            if (i == 4) continue;
            if (board[i] == O && board[8-i] == E) return 8-i;
        }
    }
    // 防穿心三连
    if (board[4] == X) {
        for (int i = 0; i < 9; i++) {
            if (i == 4) continue;
            if (board[i] == X && board[8-i] == E) return 8-i;
        }
    }

    int empty = -1;

    // 上
    for (int i = 0; i < 3; i++) {
        if (board[i] == E) {
            if (empty != -1) break;
            empty = i;
            continue;
        }
        if (board[i] == X) {
            empty = -1;
            break;
        }
    }
    if (empty != -1) return empty;
    // 防上
    for (int i = 0; i < 3; i++) {
        if (board[i] == E) {
            if (empty != -1) break;
            empty = i;
            continue;
        }
        if (board[i] == O) {
            empty = -1;
            break;
        }
    }
    if (empty != -1) return empty;

    // 下
    for (int i = 0; i < 3; i++) {
        if (board[i+6] == E) {
            if (empty != -1) break;
            empty = i;
            continue;
        }
        if (board[i+6] == X) {
            empty = -1;
            break;
        }
    }
    if (empty != -1) return empty + 6;
    // 防下
    for (int i = 0; i < 3; i++) {
        if (board[i+6] == E) {
            if (empty != -1) break;
            empty = i;
            continue;
        }
        if (board[i+6] == O) {
            empty = -1;
            break;
        }
    }
    if (empty != -1) return empty + 6;

    // 左
    for (int i = 0; i < 3; i++) {
        if (board[i*3] == E) {
            if (empty != -1) break;
            empty = i;
            continue;
        }
        if (board[i*3] == X) {
            empty = -1;
            break;
        }
    }
    if (empty != -1) return empty * 3;
    // 防左
    for (int i = 0; i < 3; i++) {
        if (board[i*3] == E) {
            if (empty != -1) break;
            empty = i;
            continue;
        }
        if (board[i*3] == O) {
            empty = -1;
            break;
        }
    }
    if (empty != -1) return empty * 3;

    // 右
    for (int i = 0; i < 3; i++) {
        if (board[i*3+2] == E) {
            if (empty != -1) break;
            empty = i;
            continue;
        }
        if (board[i*3+2] == X) {
            empty = -1;
            break;
        }
    }
    if (empty != -1) return empty * 3 + 2;
    // 防右
    for (int i = 0; i < 3; i++) {
        if (board[i*3+2] == E) {
            if (empty != -1) break;
            empty = i;
            continue;
        }
        if (board[i*3+2] == O) {
            empty = -1;
            break;
        }
    }
    if (empty != -1) return empty * 3 + 2;

    // 占中心
    if (board[4] == E) return 4;

    // 占四边
    for (int i = 1; i < 8; i += 2) {
        if (board[i] == E) {
            return i;
        }
    }

    // 其余情况
    for (int i = 0; i <= 8; i += 2) {
        if (board[i] == E) {
            return i;
        }
    }

    return -1;
}

void computerAction(void) {
    int cd = computerDecision();
    if (cd == -1) return;
    board[cd] = O;
}

int mainLoop(void) {
    initGame();
    while (1) {
        turns++;
        printf("-- Your turn (Turn %d) --\n", turns);
        playerAction();
        printBoard();
        if (hasWin(X)) {
            printf("You won!\n");
            break;
        }
        if (turns == 5) {
            printf("Draw!\n");
            break;
        }
        printf("-- Computer's turn (Turn %d) --\n", turns);
        Sleep(1000);
        computerAction();
        printBoard();
        if (hasWin(O)) {
            printf("You lost...\n");
            break;
        }
    }
    return askForPlayAgain(0);
}

int main(void) {
    printf("Welcome to Tic Tac Toe!\n");
    printTutorial();
    printf("-- Press [ENTER] to start --\n");
    while (getchar() != '\n') {}
    while (1) {
        if (mainLoop() == 0) { // 返回值0代表玩家不想继续游玩
            printf("Goodbye! (Press [ENTER] to exit.)\n");
            while (getchar() != '\n') {}
            return 0;
        }
    }
}
