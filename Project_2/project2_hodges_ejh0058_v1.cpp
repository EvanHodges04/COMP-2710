//=====================================================================
// Name        : Evan Hodges
// UserID      : ejh0058
// Filename    : project2_hodges_ejh0058.cpp
// Compile     : g++ project2_hodges_ejh0058_v1.cpp -o Project2_v1.out
// Execute     : ./Project2_v1.out
// Sources     : I did not use any external sources for this assignment
//=====================================================================

#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <ctime>
using namespace std;

// Constants
const double aaron_acc = 0.33333333;
const double bob_acc = 0.5;
const double charlie_acc = 1;
const int total_duels = 10000;

// Status
bool at_least_two_alive(bool A_alive, bool B_alive, bool C_alive) {
    int players_alive = 0;
    players_alive = A_alive ? players_alive + 1 : players_alive;
    players_alive = B_alive ? players_alive + 1 : players_alive;
    players_alive = C_alive ? players_alive + 1 : players_alive;

    return players_alive >= 2;
}

// Shooting Methods
bool shoot(double accuracy) {
    double shoot_target = ((double) rand()) / (double) RAND_MAX;
    return shoot_target <= accuracy;
}

int Aaron_shoots1(bool& B_alive, bool& C_alive) {
    if (C_alive) {
        bool hit = shoot(aaron_acc);
        C_alive = hit ? false : C_alive;
        return 2;
    }
    else if (B_alive) {
        bool hit = shoot(aaron_acc);
        B_alive = hit ? false : B_alive;
        return 1;
    }
    return -1;
}

int Aaron_shoots2(bool& B_alive, bool& C_alive) {
    if (B_alive && C_alive) {
        return -1;
    }
    return Aaron_shoots1(B_alive, C_alive);
}

int Bob_shoots(bool& A_alive, bool& C_alive) {
    if (C_alive) {
        bool hit = shoot(bob_acc);
        C_alive = hit ? false : C_alive;
        return 2;
    }
    else if (A_alive) {
        bool hit = shoot(bob_acc);
        A_alive = hit ? false : A_alive;
        return 0;
    }
    return -1;
}

int Charlie_shoots(bool& A_alive, bool& B_alive) {
    if (B_alive) {
        bool hit = shoot(charlie_acc);
        B_alive = hit ? false : B_alive;
        return 1;
    }
    else if (A_alive) {
        bool hit = shoot(charlie_acc);
        A_alive = hit ? false : A_alive;
        return 0;
    }
    return -1;
}

// Duel
int duel(int strategy) {
    bool A_alive = true;
    bool B_alive = true;
    bool C_alive = true;
    while (at_least_two_alive(A_alive, B_alive, C_alive)) {
        if (A_alive) {
            if (strategy == 1) {
                Aaron_shoots1(B_alive, C_alive);
            }
            else if (strategy == 2) {
                Aaron_shoots2(B_alive, C_alive);
            }
        }
        if (B_alive) {
            Bob_shoots(A_alive, C_alive);
        }
        if (C_alive) {
            Charlie_shoots(A_alive, B_alive);
        }
    }
    return A_alive ? 0 : B_alive ? 1 : C_alive ? 2 : -1;
}

int main() {
    // Rand
    srand(time(0));
    rand();

    cout << "*** Welcome to Evan's Duel Simulator ***\n";
    return 0;
}