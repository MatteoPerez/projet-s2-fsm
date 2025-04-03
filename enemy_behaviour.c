#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef enum {
    STATE_PATROL,
    STATE_ATTACK,
    STATE_TAKE_COVER,
    STATE_RELOAD,
    STATE_DEAD
} State;

State currentState = STATE_PATROL;

int health = 100;
int ammo = 5;
bool enemyVisible = false;
bool underFire = false;

void patrol() {
    printf("Patrolling...\n");

    // Random detection
    enemyVisible = (rand() % 3 == 0); 

    if (enemyVisible) {
        currentState = STATE_ATTACK;
    }
}

void attack() {
    if (ammo > 0) {
        printf("Attack !\n");
        ammo--;
        printf("Ammo left : %d\n", ammo);

        // Random hit taken
        underFire = (rand() % 3 == 0);

        if (underFire) {
            health -= 20;
            printf("HIt ! Health points left : %d\n", health);
            if (health <= 0) {
                currentState = STATE_DEAD;
                return;
            }
            currentState = STATE_TAKE_COVER;
        } else if (ammo == 0) {
            currentState = STATE_RELOAD;
        }
    } else {
        currentState = STATE_RELOAD;
    }
}

void takeCover() {
    printf("Taking cover !\n");
    currentState = STATE_ATTACK;
}

void reload() {
    printf("Reloading...\n");
    ammo = 10;
    currentState = STATE_ATTACK;
}

void dead() {
    printf("Guard is dead...\n");
}

void updateState() {
    switch (currentState) {
        case STATE_PATROL:
            patrol();
            break;
        case STATE_ATTACK:
            attack();
            break;
        case STATE_TAKE_COVER:
            takeCover();
            break;
        case STATE_RELOAD:
            reload();
            break;
        case STATE_DEAD:
            dead();
            break;
    }
}

int main() {
    srand(time(NULL)); // Initialise random numbers

    while (currentState != STATE_DEAD) {
        updateState();
    }

    return 0;
}
