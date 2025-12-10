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
                    cout << "Invalid move! Turn skipped.\n";
                else return move;
            } else {
                cout << "Invalid input! Turn skipped.\n";
                return -1;
            }
        }
    }

    int getRandomMove(){
        vector<int> moves;
        for(int i=0;i<9;i++) if(board[i]!=player1Mark && board[i]!=player2Mark) moves.push_back(i+1);
        return moves[rand()%moves.size()];
    }

    void makeMove(int move){ if(move!=-1) board[move-1] = currentPlayer; }

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

    void alchemistSwap() {
        string a,b;
        cout << "Enter two positions (1-9) to swap: ";
        cin >> a >> b;
        if(a.size()!=1 || b.size()!=1 || !isdigit(a[0]) || !isdigit(b[0])){
            cout << "Invalid input! Turn skipped.\n";
            return;
        }
        int p1=a[0]-'0', p2=b[0]-'0';
        if(p1<1 || p1>9 || p2<1 || p2>9 || p1==p2){
            cout << "Invalid positions! Turn skipped.\n";
            return;
        }
        swap(board[p1-1], board[p2-1]);
    }

    void paladinShift() {
        string from,to;
        cout << "Enter mark to move and target space (1-9): ";
        cin >> from >> to;
        if(from.size()!=1 || to.size()!=1 || !isdigit(from[0]) || !isdigit(to[0])){
            cout << "Invalid input! Turn skipped.\n";
            return;
        }
        int src=from[0]-'0', dst=to[0]-'0';
        if(src<1 || src>9 || dst<1 || dst>9 || src==dst){
            cout << "Invalid positions! Turn skipped.\n";
            return;
        }
        swap(board[src-1], board[dst-1]);
    }

    bool battleRound(int &playerHealth, int playerAttack, int playerDefense,
                     int &opponentHealth, int opponentAttack, int opponentDefense,
                     string opponentName = "") {

        resetBoard();
        currentPlayer = player1Mark;
        int originalPlayerDefense = playerDefense;

        while(true){
            printBoard();
            int move;
            if(currentPlayer==player1Mark){
                string choice;
                cout << "Choose: 1. Regular move 2. Special ability: ";
                cin >> choice;
                if(choice.size()!=1 || (choice[0]!='1' && choice[0]!='2')) move=-1;
                else if(choice[0]=='1') move=getPlayerMove();
                else{
                    if(player1Type=="Alchemist") alchemistSwap();
                    else if(player1Type=="Paladin") paladinShift();
                    move=-1;
                }
            } else move=getRandomMove();

            makeMove(move);

            if(checkWin()){
                if(currentPlayer==player1Mark){
                    int dmg=max(0, playerAttack-opponentDefense);
                    opponentHealth -= dmg;
                    cout << "You won this round! Opponent lost " << dmg << " health.\n";
                } else {
                    int dmg=max(0, opponentAttack-playerDefense);
                    if(opponentName=="Dragon"){
                        int roll=rand()%2;
                        if(roll==0){ cout << "Dragon uses Fire Breath! +10 damage\n"; dmg+=10; }
                        else{ cout << "Dragon uses Tail Swipe! Defense reduced 5\n"; dmg=max(0, opponentAttack-(playerDefense-5)); }
                    }
                    playerHealth -= dmg;
                    cout << "Opponent won this round! You lost " << dmg << " health.\n";
                    playerDefense=originalPlayerDefense;
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
        currentPlayer='X';
        while(true){
            printBoard();
            int move=getPlayerMove();
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
            currentPlayer=(currentPlayer=='X')?'O':'X';
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

int getStrictInput(vector<int> validChoices){
    string input;
    while(true){
        cin >> input;
        if(input.size()==1 && isdigit(input[0])){
            int val=input[0]-'0';
            for(int v:validChoices) if(val==v) return val;
        }
        cout << "Invalid input! Enter only one valid number: ";
    }
}

void event(Character &player){
    cout << "\n--- Event ---\n";
    cout << "1. Drink potion (+20 health)\n2. Train (+5 attack,+5 defense)\n> ";
    int choice=getStrictInput({1,2});
    if(choice==1){ player.health+=20; cout << "Health: " << player.health << "\n"; }
    else { player.attack+=5; player.defense+=5; cout << "Attack: " << player.attack << ", Defense: " << player.defense << "\n"; }
}

void campaign(){
    srand(time(0));
    Character player;
    cout << "Enter your hero's name: "; cin >> player.name;

    cout << "Choose class:\n1. Paladin\n2. Alchemist\n> ";
    int classChoice=getStrictInput({1,2});
    if(classChoice==1){ player.type="Paladin"; player.health=100; player.attack=15; player.defense=10; }
    else { player.type="Alchemist"; player.health=100; player.attack=12; player.defense=8; }
    player.gold=50;

    vector<Character> foes={{"Goblin","Alchemist",40,8,5,0},{"Orc","Paladin",50,12,8,0},
                            {"Dark Knight","Paladin",60,15,10,0},{"Sorcerer","Alchemist",55,14,9,0},
                            {"Dragon","Paladin",100,20,12,0}};

    for(int i=0;i<foes.size();i++){
        Character &foe=foes[i];
        cout << "\n--- Battle " << i+1 << ": " << foe.name << " ---\n";
        TicTacToe battle;
        battle.setupBattleMode('X','O',player.type,foe.type);

        if(foe.name=="Dragon"){
            while(player.health>0 && foe.health>0)
                battle.battleRound(player.health,player.attack,player.defense,
                                   foe.health,foe.attack,foe.defense,foe.name);
        } else {
            while(player.health>0 && foe.health>0)
                battle.battleRound(player.health,player.attack,player.defense,
                                   foe.health,foe.attack,foe.defense);
        }

        if(player.health<=0){ cout << "You have been defeated! Campaign restarts.\n"; return; }
        else{ cout << "You defeated " << foe.name << "!\n"; player.gold+=20; if(i<foes.size()-1) event(player);}
    }
    cout << "\nCongratulations " << player.name << "! You completed the campaign!\n";
}

int main(){
    srand(time(0));
    TicTacToe game;

    while(true){
        cout << "\n=== Tic-Tac-Toe Menu ===\n1. Player vs Player\n2. Battle PvP with classes\n3. Campaign/RPG\n4. Exit\n> ";
        int choice=getStrictInput({1,2,3,4});

        if(choice==1) game.playPvP();
        else if(choice==2){
            char p1Mark,p2Mark; string p1Type,p2Type;
            cout << "Player 1, choose mark: "; cin >> p1Mark;
            cout << "Player 1, choose class: 1. Paladin 2. Alchemist\n> "; p1Type=(getStrictInput({1,2})==1)?"Paladin":"Alchemist";
            cout << "Player 2, choose mark: "; cin >> p2Mark;
            cout << "Player 2, choose class: 1. Paladin 2. Alchemist\n> "; p2Type=(getStrictInput({1,2})==1)?"Paladin":"Alchemist";

            game.setupBattleMode(p1Mark,p2Mark,p1Type,p2Type);
            game.playPvP();
        } else if(choice==3) campaign();
        else break;
    }
    cout << "Thanks for playing!\n";
    return 0;
}
