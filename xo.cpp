// ### XO game written in C++ ###

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
using namespace std;

#define wait(ms) (this_thread::sleep_for(chrono::milliseconds(ms)))
#define cls (cout << "\033[2J\033[H")
#define red "\033[31m"
#define blue "\033[34m"
#define reset "\033[0m"

char grid[9], winner;

// intro animation
void intro() {
    cout << "starting." << flush;
    wait(500);
    cls;
    cout << "starting.." << flush;
    wait(500);
    cls;
    cout << "starting..." << endl;
    wait(500);
    cout << "welcome to terminal XO." << flush;
    wait(500);
    cls;
}

// prints the board. empty cells show their number [1-9], taken cells show X or O.
void printGrid() {
    cout << "\n " << string(13, '-') << '\n';
    for (int i = 0; i <= 8; i++) {
        if (grid[i]) cout << " | " << (grid[i] == 'X' ? red : blue) << grid[i] << reset;
        else cout << " | " << (i + 1);
        if (i == 2 || i == 5 || i == 8) cout << " |\n " << string(13, '-') << '\n';
    }
    cout << '\n';
}

// checks for a win or draw. sets winner and returns true if the game is over.
bool checkGameOver() {
    winner = 0;
    
    // check rows
    for (int i = 0; i <= 6; i += 3) {
        if (grid[i] && grid[i] == grid[i + 1] && grid[i] == grid[i + 2]) {
            winner = grid[i];
            return true;
        }
    }
    
    // check columns
    for (int i = 0; i <= 2; i++) {
        if (grid[i] && grid[i] == grid[i + 3] && grid[i] == grid[i + 6]) {
            winner = grid[i];
            return true;
        }
    }
    
    // check diagonals
    if (grid[4] && ((grid[4] == grid[0] && grid[4] == grid[8]) || (grid[4] == grid[2] && grid[4] == grid[6]))) {
        winner = grid[4];
        return true;
    }
    
    // draw: board is full and no winner
    for (int i = 0; i <= 8; i++) if (!grid[i]) return false;
    return true;
}

// AI: tries to win, blocks the human, or plays a random empty cell.
void play(char ai) {
    char human = (ai == 'O' ? 'X' : 'O');
    
    // 1. try winning move
    for (int i = 0; i <= 8; i++) {
        if (!grid[i]) {
            grid[i] = ai;
            if (checkGameOver() && winner) {
                wait(500);
                cout << i + 1 << flush;
                wait(1000);
                return;
            }
            grid[i] = 0;
        }
    }
    
    // 2. block human
    for (int i = 0; i <= 8; i++) {
        if (!grid[i]) {
            grid[i] = human;
            if (checkGameOver() && winner) {
                grid[i] = ai;
                wait(500);
                cout << i + 1 << flush;
                wait(1000);
                return;
            }
            grid[i] = 0;
        }
    }
    
    // 3. otherwise random
    int num;
    do num = rand() % 9; while (grid[num]);
    grid[num] = ai;
    wait(500);
    cout << num + 1 << flush;
    wait(1000);
}

int main() {
    intro();
    
    string input;
    srand(chrono::steady_clock::now().time_since_epoch().count()); // uses current time as a seed
    
    while (true) {
        bool solo = true, startFirst = true;
        cout << "play solo? [Y/n] ";
        getline(cin, input);
        if (input == "n" || input == "N") solo = false;
        else {
            cout << "start first? [Y/n] ";
            getline(cin, input);
            if (input == "n" || input == "N") startFirst = false;
        }
        
        // 'i' tracks turns: even = X, odd = O
        for (int i = 0; !checkGameOver(); i++) {
            cls;
            printGrid();
            
            char player = (i % 2 == 0 ? 'X' : 'O');
            cout << (player == 'X' ? red : blue) << player << reset << "'s turn. [1-9] " << flush;
            
            if (solo && player == (startFirst ? 'O' : 'X')) play(player);
            else {
                int position;
                if (!(cin >> position) || position > 9 || position < 1 || grid[position - 1]) {
                    cout << "invalid input, try again." << flush;
                    cin.clear();
                    i--;
                    wait(1000);
                } else grid[position - 1] = player;
                cin.ignore(100, '\n');
            }
        }
        
        cls;
        printGrid();
        winner ? cout << (winner == 'X' ? red : blue) << winner << reset << " won the game." << flush : cout << "it's a draw." << flush;
        wait(1000);
        cout << " play again? [Y/n] ";
        getline(cin, input);
        if (input == "n" || input == "N") break;
        
        // reset for next game
        for (char &c : grid) c = 0;
        cls;
    }
    
    cls;
    cout << "goodbye." << flush;
    wait(1000);
    cls;
}
