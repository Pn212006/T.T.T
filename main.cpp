#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

class TicTacToe {
private:
    vector<char> board;
    char currentPlayer;
    bool gameOver;

public:
    TicTacToe() {
        board = {'1','2','3','4','5','6','7','8','9'};
        currentPlayer = 'X';
        gameOver = false;
    }

    void printBoard() {
        cout << "\n";
        for (int i = 0; i < 9; i++) {
            cout << " " << board[i] << " ";
            if (i % 3 != 2) cout << "|";
            if (i % 3 == 2 && i != 8) {
                cout << "\n---+---+---\n";
            }
        }
        cout << "\n\n";
    }

    // Separate input validation
    int getPlayerMove() {
        string input;
        int move;

        while (true) {
            cout << "Player " << currentPlayer << ", enter your move (1-9): ";
            cin >> input;

            // check that input is a single digit
            if (input.size() == 1 && isdigit(input[0])) {
                move = input[0] - '0';

                if (move < 1 || move > 9) {
                    cout << "Invalid move! Out of range. Try again.\n";
                } else if (board[move - 1] == 'X' || board[move - 1] == 'O') {
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
            if (c != 'X' && c != 'O') return false;
        }
        return true;
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    // Separate result checking
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
        while (!gameOver) {
            printBoard();
            int move = getPlayerMove();
            makeMove(move);
            if (checkGameResult()) {
                gameOver = true;
            } else {
                switchPlayer();
            }
        }
    }
};

int main() {
    cout << "Welcome to Tic-Tac-Toe!\n";
    TicTacToe game;
    game.play();
    cout << "Game over. Thanks for playing!\n";
    return 0;
}
