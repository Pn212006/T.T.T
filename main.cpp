#include <iostream>
#include <vector>
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

    bool isValidMove(int move) {
        return (move >= 1 && move <= 9 && board[move - 1] != 'X' && board[move - 1] != 'O');
    }

    void makeMove(int move) {
        board[move - 1] = currentPlayer;
    }

    bool checkWin() {
        // Winning combinations
        int wins[8][3] = {
            {0,1,2}, {3,4,5}, {6,7,8}, // rows
            {0,3,6}, {1,4,7}, {2,5,8}, // cols
            {0,4,8}, {2,4,6}           // diagonals
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

    void play() {
        while (!gameOver) {
            printBoard();
            cout << "Player " << currentPlayer << ", enter your move (1-9): ";

            int move;
            if (!(cin >> move)) {
                cout << "Invalid input! Please enter a number 1-9.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (!isValidMove(move)) {
                cout << "Invalid move! Try again.\n";
                continue;
            }

            makeMove(move);

            if (checkWin()) {
                printBoard();
                cout << "Player " << currentPlayer << " wins!\n";
                gameOver = true;
            } else if (checkTie()) {
                printBoard();
                cout << "It's a tie!\n";
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
