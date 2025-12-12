#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <fstream>
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
                if(move<1||move>9 || board[move-1]==player1Mark || board[move-1]==player2Mark) {
                    cout << "Invalid move! Turn skipped.\n";
                    return -1;
                } else return move;
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

    bool battleRound(int &playerHealth, int &playerAttack, int &playerDefense,
                     int &opponentHealth, int &opponentAttack, int &opponentDefense,
                     string opponentName = "", int opponentId = -1) {

        resetBoard();
        currentPlayer = player1Mark;
        int originalPlayerDefense = playerDefense;
        int originalOpponentAttack = opponentAttack;
        int originalOpponentDefense = opponentDefense;

        while(true){
            printBoard();
            int move;
            if(currentPlayer==player1Mark){
                string choice;
                cout << "Choose: 1. Regular move 2. Use special ability\n> ";
                cin >> choice;
                if(choice.size()!=1 || (choice[0]!='1' && choice[0]!='2')) {
                    cout << "Invalid input! Turn skipped.\n";
                    move = -1;
                } else if(choice[0]=='1') move=getPlayerMove();
                else {
                    if(player1Type=="Alchemist") alchemistSwap();
                    else if(player1Type=="Paladin") paladinShift();
                    move = -1;
                }
            } else {
                int r = rand()%3;
                if(r<2) move = getRandomMove();
                else {
                    if(player2Type=="Alchemist") alchemistSwap();
                    else if(player2Type=="Paladin") paladinShift();
                    move = -1;
                }
            }

            makeMove(move);

            if(checkWin()){
                if(currentPlayer==player1Mark){
                    int raw = playerAttack - opponentDefense;
                    int dmg = max(1, raw);
                    opponentHealth -= dmg;
                    cout << "You won this round! Opponent lost " << dmg << " health.\n";
                } else {
                    int raw = opponentAttack - playerDefense;
                    int dmg = max(1, raw);
                    if(opponentName!=""){
                        int roll = rand()%3;
                        if(opponentName=="Goblin" && roll==0){ cout<<"Goblin uses Sneak Strike! +3 damage\n"; dmg +=3; }
                        else if(opponentName=="Orc" && roll==0){ cout<<"Orc uses Berserk! +5 attack this round\n"; dmg +=5; }
                        else if(opponentName=="Dark Knight" && roll==0){ cout<<"Dark Knight uses Armor Crush! -3 defense to you next round\n"; playerDefense = max(0, playerDefense-3); dmg = max(1, opponentAttack-playerDefense); }
                        else if(opponentName=="Sorcerer" && roll==0){ cout<<"Sorcerer uses Drain! Heals for half damage dealt\n"; int heal = dmg/2; opponentHealth += heal; cout<<"Sorcerer healed "<<heal<<"\n"; }
                        else if(opponentName=="Dragon"){
                            int abilityRoll = rand()%3;
                            if(abilityRoll==0){ cout<<"Dragon uses Fire Breath! Extra 10 damage!\n"; dmg += 10; }
                            else if(abilityRoll==1){ cout<<"Dragon uses Tail Swipe! Your defense reduced by 5 for this round!\n"; playerDefense = max(0, playerDefense-5); dmg = max(1, opponentAttack-playerDefense); }
                            else { cout<<"Dragon uses Roar! It heals itself for 8 health!\n"; opponentHealth += 8; }
                        }
                    }
                    playerHealth -= dmg;
                    cout << "Opponent won this round! You lost " << dmg << " health.\n";
                    playerDefense = originalPlayerDefense;
                    opponentAttack = originalOpponentAttack;
                    opponentDefense = originalOpponentDefense;
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

class Player {
public:
    string name, type;
    int health, attack, defense, gold;

    Player() : name(""), type(""), health(0), attack(0), defense(0), gold(0) {}
    Player(string n, string t, int h, int a, int d, int g)
        : name(n), type(t), health(h), attack(a), defense(d), gold(g) {}
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

void shop(Player &player){
    cout << "\n--- Shop ---\n";
    cout << "Gold: " << player.gold << "\n";
    cout << "1. Small Potion (+25 HP) - 20 gold\n";
    cout << "2. Attack Tonic (+3 Attack) - 30 gold\n";
    cout << "3. Defense Tonic (+3 Defense) - 30 gold\n";
    cout << "4. Leave\n> ";
    int choice = getStrictInput({1,2,3,4});
    if(choice==1){
        if(player.gold>=20){ player.gold-=20; player.health+=25; cout<<"Healed 25 HP. Health: "<<player.health<<"\n"; }
        else cout<<"Not enough gold.\n";
    } else if(choice==2){
        if(player.gold>=30){ player.gold-=30; player.attack+=3; cout<<"Attack increased. Attack: "<<player.attack<<"\n"; }
        else cout<<"Not enough gold.\n";
    } else if(choice==3){
        if(player.gold>=30){ player.gold-=30; player.defense+=3; cout<<"Defense increased. Defense: "<<player.defense<<"\n"; }
        else cout<<"Not enough gold.\n";
    } else cout<<"Leaving shop.\n";
}

void event(Player &player){
    cout << "\n--- Event ---\n";
    cout << "1. Drink potion (+20 health)\n2. Train (+5 attack,+5 defense)\n3. Take the risky path (may lose or gain)\n4. Visit shop\n> ";
    int choice = getStrictInput({1,2,3,4});
    if(choice==1){ player.health+=20; cout<<"Health: "<<player.health<<"\n"; }
    else if(choice==2){ player.attack+=5; player.defense+=5; cout<<"Attack: "<<player.attack<<", Defense: "<<player.defense<<"\n"; }
    else if(choice==3){
        int r=rand()%3;
        if(r==0){ player.health = max(0, player.health-15); cout<<"You tripped! Health now "<<player.health<<"\n"; }
        else if(r==1){ player.attack+=4; cout<<"You found a relic! Attack now "<<player.attack<<"\n"; }
        else { player.gold+=25; cout<<"You scrounged some gold! Gold now "<<player.gold<<"\n"; }
    } else shop(player);
}

bool saveGame(const Player &player, int battleIndex, const vector<Player> &foes){
    ofstream f("savegame.txt");
    if(!f) return false;
    f<<player.name<<"\n";
    f<<player.type<<"\n";
    f<<player.health<<" "<<player.attack<<" "<<player.defense<<" "<<player.gold<<"\n";
    f<<battleIndex<<"\n";
    for(auto &foe:foes) f<<foe.name<<" "<<foe.health<<"\n";
    f.close();
    return true;
}

bool loadGame(Player &player, int &battleIndex, vector<Player> &foes){
    ifstream f("savegame.txt");
    if(!f) return false;
    string line;
    getline(f, player.name);
    getline(f, player.type);
    f>>player.health>>player.attack>>player.defense>>player.gold;
    f>>battleIndex;
    string fname; int fhealth;
    for(size_t i=0;i<foes.size();i++){
        if(!(f>>fname>>fhealth)) break;
        foes[i].health = fhealth;
    }
    f.close();
    return true;
}

void campaign(){
    srand(time(0));
    Player player;
    cout << "Enter your hero's name: "; cin >> player.name;
    cout << "Load saved campaign? 1. Yes 2. No\n> ";
    int loadChoice = getStrictInput({1,2});
    vector<Player> foes = {
        Player("Goblin","Alchemist",40,8,5,0),
        Player("Orc","Paladin",50,12,8,0),
        Player("Dark Knight","Paladin",60,15,10,0),
        Player("Sorcerer","Alchemist",55,14,9,0),
        Player("Dragon","Paladin",120,22,14,0)
    };

    int startIndex = 0;
    if(loadChoice==1){
        int idx=0;
        bool ok = loadGame(player, idx, foes);
        if(ok){
            cout<<"Save loaded. Continue where you left off.\n";
            startIndex = idx;
        } else cout<<"No save found. Starting new campaign.\n";
    }
    if(loadChoice==2){
        cout << "Choose class:\n1. Paladin\n2. Alchemist\n> ";
        int classChoice = getStrictInput({1,2});
        if(classChoice==1) player = Player(player.name,"Paladin",100,15,10,50);
        else player = Player(player.name,"Alchemist",100,12,8,50);
    }

    vector<vector<int>> pathOptions = {{0,1,2,3,4},{0,2,1,3,4}};
    int chosenPath = 0;

    for(int i=startIndex;i<5;i++){
        int foeIndex = pathOptions[chosenPath][i];
        Player foe = foes[foeIndex];
        cout << "\n--- Battle " << i+1 << ": " << foe.name << " ---\n";
        TicTacToe battle;
        battle.setupBattleMode('X','O',player.type,foe.type);

        while(player.health>0 && foe.health>0){
            battle.battleRound(player.health, player.attack, player.defense,
                               foe.health, foe.attack, foe.defense, foe.name, foeIndex);
        }

        if(player.health<=0){
            cout<<"You have been defeated! Campaign restarts.\n";
            return;
        } else {
            cout<<"You defeated "<<foe.name<<"!\n";
            player.gold += 20;
            if(i<4){
                cout<<"\nChoose path for next step: 1. Safer route (less gold) 2. Risky route (more gold)\n> ";
                int pc = getStrictInput({1,2});
                if(pc==1) chosenPath=0; else chosenPath=1;
                event(player);
            }
            cout<<"You now have "<<player.gold<<" gold.\n";
            cout<<"Save progress? 1. Yes 2. No\n> ";
            int sc = getStrictInput({1,2});
            if(sc==1){
                vector<Player> foesToSave = foes;
                int battleIndex = i+1;
                bool ok = saveGame(player, battleIndex, foesToSave);
                if(ok) cout<<"Game saved.\n"; else cout<<"Save failed.\n";
            }
        }
    }

    cout<<"\nCongratulations "<<player.name<<"! You completed the campaign!\n";
    remove("savegame.txt");
}

int main(){
    srand(time(0));
    TicTacToe game;

    while(true){
        cout << "\n=== Tic-Tac-Toe Menu ===\n1. Player vs Player\n2. Battle PvP with classes\n3. Campaign/RPG\n4. Exit\n> ";
        int choice = getStrictInput({1,2,3,4});

        if(choice==1) game.playPvP();
        else if(choice==2){
            char p1Mark,p2Mark; string p1Type,p2Type;
            cout << "Player 1, choose mark (single char): "; cin >> p1Mark;
            cout << "Player 1, choose class: 1. Paladin 2. Alchemist\n> ";
            p1Type = (getStrictInput({1,2})==1)?"Paladin":"Alchemist";
            cout << "Player 2, choose mark (single char): "; cin >> p2Mark;
            cout << "Player 2, choose class: 1. Paladin 2. Alchemist\n> ";
            p2Type = (getStrictInput({1,2})==1)?"Paladin":"Alchemist";
            game.setupBattleMode(p1Mark,p2Mark,p1Type,p2Type);
            game.playPvP();
        } else if(choice==3) campaign();
        else break;
    }
    cout << "Thanks for playing!\n";
    return 0;
}
