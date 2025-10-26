#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

class TicTacToe {
private:
    vector<char> board;
    char currentPlayer;
    bool gameOver;
    char player1Mark, player2Mark;
    string player1Type, player2Type;
    bool battleMode;

public:
    TicTacToe() {
        board = {'1','2','3','4','5','6','7','8','9'};
        currentPlayer = 'X';
        gameOver = false;
        battleMode = false;
    }

    void resetBoard() {
        board = {'1','2','3','4','5','6','7','8','9'};
    }

    void printBoard() {
        cout << "\n";
        for (int i = 0; i < 9; i++) {
            cout << " " << board[i] << " ";
            if (i % 3 != 2) cout << "|";
            if (i % 3 == 2 && i != 8) cout << "\n---+---+---\n";
        }
        cout << "\n\n";
    }

    void mainMenu() {
        while (true) {
            cout << "Welcome to Tic-Tac-Toe!\n";
            cout << "1. Regular Tic-Tac-Toe\n";
            cout << "2. Battle Tic-Tac-Toe\n";
            cout << "Choose a mode (1 or 2): ";
            string choice;
            getline(cin, choice);

            if (choice == "1") {
                battleMode = false;
                player1Mark = 'X';
                player2Mark = 'O';
                play();
                break;
            } else if (choice == "2") {
                battleMode = true;
                setupBattleMode();
                play();
                break;
            } else {
                cout << "Invalid option! Try again.\n";
            }
        }
    }

    void setupBattleMode() {
        cout << "\n--- Battle Tic-Tac-Toe Setup ---\n";
        player1Mark = getCustomMark(1);
        player1Type = getArchetype(1);
        player2Mark = getCustomMark(2);
        player2Type = getArchetype(2);
        cout << "\nPlayer 1: " << player1Mark << " (" << player1Type << ")\n";
        cout << "Player 2: " << player2Mark << " (" << player2Type << ")\n\n";
        currentPlayer = player1Mark;
    }

    char getCustomMark(int playerNum) {
        string input;
        while (true) {
            cout << "Player " << playerNum << ", choose your mark (A-Z, a-z, ?, !, *, ~, $, %, #): ";
            getline(cin, input);
            if (input.size() == 1 && (isalnum(input[0]) || string("?*!~$%#").find(input[0]) != string::npos))
                return input[0];
            cout << "Invalid mark! Try again.\n";
        }
    }

    string getArchetype(int playerNum) {
        string type;
        while (true) {
            cout << "Player " << playerNum << ", choose archetype (Paladin / Alchemist): ";
            getline(cin, type);
            for (auto &c : type) c = tolower(c);
            if (type == "paladin" || type == "alchemist") return type;
            cout << "Invalid archetype! Try again.\n";
        }
    }

    int getPlayerMove() {
        string input;
        int move;
        while (true) {
            cout << "Player " << currentPlayer << ", enter your move (1-9): ";
            cin >> input;
            if (input.size() == 1 && isdigit(input[0])) {
                move = input[0] - '0';
                if (move < 1 || move > 9) {
                    cout << "Invalid move! Out of range. Try again.\n";
                } else if (board[move - 1] == player1Mark || board[move - 1] == player2Mark || 
                           board[move - 1] == 'X' || board[move - 1] == 'O') {
                    cout << "Invalid move! That square is already taken. Try again.\n";
                } else {
                    return move;
                }
            } else {
                cout << "Invalid input! Please enter a single number between 1 and 9.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    void makeMove(int move) {
        board[move - 1] = currentPlayer;
    }

    bool checkWin() {
        int wins[8][3] = {
            {0,1,2}, {3,4,5}, {6,7,8},
            {0,3,6}, {1,4,7}, {2,5,8},
            {0,4,8}, {2,4,6}
        };
        for (auto &win : wins) {
            if (board[win[0]] == currentPlayer &&
                board[win[1]] == currentPlayer &&
                board[win[2]] == currentPlayer) {
                return true;
            }
        }
        return false;
    }

    bool checkTie() {
        for (char c : board) {
            if (c != player1Mark && c != player2Mark && c != 'X' && c != 'O') return false;
        }
        return true;
    }

    void switchPlayer() {
        if (battleMode) {
            currentPlayer = (currentPlayer == player1Mark) ? player2Mark : player1Mark;
        } else {
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    void alchemistSwap() {
        string a, b;
        cout << "Enter two positions (1-9) to swap:\n";
        cin >> a >> b;
        if (a.size() == 1 && b.size() == 1 && isdigit(a[0]) && isdigit(b[0])) {
            int p1 = a[0] - '0';
            int p2 = b[0] - '0';
            if (p1 < 1 || p1 > 9 || p2 < 1 || p2 > 9) {
                cout << "Invalid positions.\n"; return;
            }
            if (board[p1-1] == board[p2-1]) {
                cout << "Cannot swap identical marks.\n"; return;
            }
            swap(board[p1-1], board[p2-1]);
        } else {
            cout << "Invalid input.\n";
        }
    }

    void paladinShift() {
        string from, to;
        cout << "Enter mark to move and target space:\n";
        cin >> from >> to;
        if (from.size() == 1 && to.size() == 1 && isdigit(from[0]) && isdigit(to[0])) {
            int src = from[0]-'0', dst = to[0]-'0';
            if (src < 1 || src > 9 || dst < 1 || dst > 9) {
                cout << "Invalid positions.\n"; return;
            }
            if (board[dst-1] == player1Mark || board[dst-1] == player2Mark) {
                cout << "Destination occupied.\n"; return;
            }
            if (!isAdjacent(src, dst)) {
                cout << "Can only move to adjacent squares.\n"; return;
            }
            swap(board[src-1], board[dst-1]);
        } else {
            cout << "Invalid input.\n";
        }
    }

    bool isAdjacent(int a, int b) {
        int rowA = (a-1)/3, colA = (a-1)%3;
        int rowB = (b-1)/3, colB = (b-1)%3;
        return (abs(rowA-rowB) <= 1 && abs(colA-colB) <= 1 && a != b);
    }

    bool checkGameResult() {
        if (checkWin()) {
            printBoard();
            cout << "Player " << currentPlayer << " wins!\n";
            return true;
        } else if (checkTie()) {
            printBoard();
            cout << "It's a tie!\n";
            return true;
        }
        return false;
    }

    void play() {
        resetBoard();
        gameOver = false;
        if (battleMode) currentPlayer = player1Mark;
        while (!gameOver) {
            printBoard();
            if (battleMode) {
                string type = (currentPlayer == player1Mark) ? player1Type : player2Type;
                cout << "Player " << currentPlayer << " (" << type << ") choose:\n";
                cout << "1. Regular move\n2. Use special ability\n> ";
                string choice;
                cin >> choice;
                if (choice == "2") {
                    if (type == "alchemist") alchemistSwap();
                    else if (type == "paladin") paladinShift();
                    else cout << "Invalid archetype.\n";
                } else {
                    int move = getPlayerMove();
                    makeMove(move);
                }
            } else {
                int move = getPlayerMove();
                makeMove(move);
            }
            if (checkGameResult()) {
                gameOver = true;
            } else {
                switchPlayer();
            }
        }
        string again;
        cout << "Play again? (y/n): ";
        cin >> again;
        if (again == "y" || again == "Y") {
            mainMenu();
        } else {
            cout << "Thanks for playing!\n";
        }
    }
};

int main() {
    TicTacToe game;
    game.mainMenu();
    return 0;
}
