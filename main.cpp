#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;

class TicTacToe {
private:
    vector<char> board;
    char currentPlayer;
    char player1Mark, player2Mark;
    string player1Type, player2Type;
    bool battleMode;

public:
    TicTacToe() {
        board = {'1','2','3','4','5','6','7','8','9'};
        currentPlayer = 'X';
        battleMode = false;
    }

    void resetBoard() { board = {'1','2','3','4','5','6','7','8','9'}; }

    void printBoard() {
        cout << "\n";
        for(int i=0;i<9;i++){
            cout << " " << board[i] << " ";
            if(i%3!=2) cout<<"|";
            if(i%3==2 && i!=8) cout<<"\n---+---+---\n";
        }
        cout << "\n";
    }

    void setupBattleMode(char pMark, char oMark, string pType, string oType){
        battleMode = true;
        player1Mark = pMark;
        player2Mark = oMark;
        player1Type = pType;
        player2Type = oType;
        currentPlayer = player1Mark;
    }

    int getPlayerMove() {
        string input;
        int move;
        while(true){
            cout << "Player " << currentPlayer << ", enter move (1-9): ";
            cin >> input;
            if(input.size()==1 && isdigit(input[0])){
                move = input[0]-'0';
                if(move<1||move>9 || board[move-1]==player1Mark || board[move-1]==player2Mark)
                    cout << "Invalid move!\n";
                else return move;
            } else cout << "Invalid input!\n";
        }
    }

    int getRandomMove(){
        vector<int> moves;
        for(int i=0;i<9;i++) if(board[i]!='X' && board[i]!='O') moves.push_back(i+1);
        return moves[rand()%moves.size()];
    }

    void makeMove(int move){ board[move-1] = currentPlayer; }

    bool checkWin() {
        int wins[8][3] = {
            {0,1,2},{3,4,5},{6,7,8},
            {0,3,6},{1,4,7},{2,5,8},
            {0,4,8},{2,4,6}
        };
        for(auto &w:wins)
            if(board[w[0]]==currentPlayer && board[w[1]]==currentPlayer && board[w[2]]==currentPlayer)
                return true;
        return false;
    }

    bool checkTie(){
        for(char c:board) if(c!=player1Mark && c!=player2Mark) return false;
        return true;
    }

    void switchPlayer() { currentPlayer = (currentPlayer==player1Mark)?player2Mark:player1Mark; }

    
    bool battleRound(int &playerHealth, int playerAttack, int playerDefense,
                     int &opponentHealth, int opponentAttack, int opponentDefense){
        resetBoard();
        currentPlayer = player1Mark;
        while(true){
            printBoard();
            int move = (currentPlayer==player1Mark)? getPlayerMove() : getRandomMove();
            makeMove(move);
            if(checkWin()){
                if(currentPlayer==player1Mark){
                    int dmg = max(0, playerAttack-opponentDefense);
                    opponentHealth -= dmg;
                    cout << "You won this round! Opponent lost " << dmg << " health.\n";
                } else {
                    int dmg = max(0, opponentAttack-playerDefense);
                    playerHealth -= dmg;
                    cout << "Opponent won this round! You lost " << dmg << " health.\n";
                }
                return (playerHealth>0 && opponentHealth>0);
            } else if(checkTie()){
                cout << "Round tied! No damage.\n";
                return true;
            }
            switchPlayer();
        }
    }

    
    void playPvP() {
        resetBoard();
        currentPlayer = 'X';
        while(true){
            printBoard();
            int move = getPlayerMove();
            makeMove(move);
            if(checkWin()){
                printBoard();
                cout << "Player " << currentPlayer << " wins!\n";
                break;
            } else if(checkTie()){
                printBoard();
                cout << "It's a tie!\n";
                break;
            }
            currentPlayer = (currentPlayer=='X')?'O':'X';
        }
    }
};


struct Character {
    string name;
    string type;
    int health;
    int attack;
    int defense;
    int gold;
};

void event(Character &player){
    cout << "\n--- Event ---\n";
    cout << "You find a potion or a training dummy.\n";
    cout << "1. Drink potion (+20 health)\n2. Train (+5 attack, +5 defense)\n> ";
    int choice;
    cin >> choice;
    if(choice==1){
        player.health += 20;
        cout << "Health is now " << player.health << "\n";
    } else {
        player.attack += 5;
        player.defense += 5;
        cout << "Attack: " << player.attack << ", Defense: " << player.defense << "\n";
    }
}


void campaign(){
    srand(time(0));
    Character player;
    cout << "Enter your hero's name: ";
    cin >> player.name;
    cout << "Choose class (Paladin/Alchemist): ";
    cin >> player.type;
    player.health = 100;
    player.attack = (player.type=="Paladin")? 15:12;
    player.defense = (player.type=="Paladin")? 10:8;
    player.gold = 50;

    vector<Character> foes = {
        {"Goblin","Alchemist",40,8,5,0},
        {"Orc","Paladin",50,12,8,0},
        {"Dark Knight","Paladin",60,15,10,0},
        {"Sorcerer","Alchemist",55,14,9,0},
        {"Dragon","Paladin",100,20,12,0} // final boss
    };

    for(int i=0;i<foes.size();i++){
        Character &foe = foes[i];
        cout << "\n--- Battle " << i+1 << ": " << foe.name << " ---\n";
        TicTacToe battle;
        battle.setupBattleMode('X','O',player.type,foe.type);
        while(player.health>0 && foe.health>0)
            battle.battleRound(player.health, player.attack, player.defense,
                               foe.health, foe.attack, foe.defense);

        if(player.health<=0){
            cout << "You have been defeated! Campaign restarts.\n";
            return;
        } else {
            cout << "You defeated " << foe.name << "!\n";
            player.gold += 20;
            if(i<foes.size()-1) event(player);
        }
    }
    cout << "\nCongratulations " << player.name << "! You completed the campaign!\n";
}


int main(){
    srand(time(0));
    TicTacToe game;

    while(true){
        cout << "\n=== Tic-Tac-Toe Menu ===\n";
        cout << "1. Player vs Player\n";
        cout << "2. Battle Tic-Tac-Toe (PvP with classes)\n";
        cout << "3. Campaign/RPG Mode\n";
        cout << "4. Exit\n";
        cout << "Choose an option: ";
        int choice;
        cin >> choice;

        if(choice==1){
            game.playPvP();
        } else if(choice==2){
            string p1Type, p2Type;
            char p1Mark, p2Mark;
            cout << "Player 1, choose mark: "; cin >> p1Mark;
            cout << "Player 1, choose class (Paladin/Alchemist): "; cin >> p1Type;
            cout << "Player 2, choose mark: "; cin >> p2Mark;
            cout << "Player 2, choose class (Paladin/Alchemist): "; cin >> p2Type;
            game.setupBattleMode(p1Mark, p2Mark, p1Type, p2Type);
            game.playPvP();
        } else if(choice==3){
            campaign();
        } else break;
    }
    cout << "Thanks for playing!\n";
    return 0;
}
