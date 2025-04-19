// Cooperative multitasking
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>


typedef enum {
    
    TAKE_COVER,
    
    ATTACK,
    
    PATROL,
    
    RELOAD,
    
    DEAD,
    
    WINNER,
    
    IDLE,
    
    PATROL,
    
    WINNER,
    
    DEAD,
    
} global_state;

static global_state global_current_state = PATROL;
static bool global_state_entered = false; // Tracks if the state was just entered

void global_step() {
    if (!global_state_entered) {
        switch (global_current_state) {
            
            case TAKE_COVER:
                
                printf("Le soldat se cache.\n");
                
                global_state_entered = true; // Mark the state as entered
                break;
            
            case ATTACK:
                
                printf("Le soldat tire.\n");
                ammo -= 1;
                
                global_state_entered = true; // Mark the state as entered
                break;
            
            case PATROL:
                
                printf("Le soldat est en patrouille.\n");
                
                global_state_entered = true; // Mark the state as entered
                break;
            
            case RELOAD:
                
                printf("Le soldat recharge.\n");
                ammo = max_ammo;
                
                global_state_entered = true; // Mark the state as entered
                break;
            
            case DEAD:
                
                printf("Le soldat est mort.\n");
                
                global_state_entered = true; // Mark the state as entered
                break;
            
            case WINNER:
                
                printf("Le joueur a péri.\n");
                
                global_state_entered = true; // Mark the state as entered
                break;
            
            case IDLE:
                
                printf("Tourelle en attente.\n");
                
                global_state_entered = true; // Mark the state as entered
                break;
            
            case PATROL:
                
                printf("Tourelle en patrouille\n");
                
                global_state_entered = true; // Mark the state as entered
                break;
            
            case WINNER:
                
                printf("Le joueur est mort.\n");
                
                global_state_entered = true; // Mark the state as entered
                break;
            
            case DEAD:
                
                printf("Tourelle HS !\n");
                
                global_state_entered = true; // Mark the state as entered
                break;
            
        }
    }

    switch (global_current_state) {
        
        case TAKE_COVER:
            
            if (ammo <= 0) {
                global_current_state = RELOAD;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat recharge.\n");
                ammo = max_ammo;
                
                break;
            }
            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat tire.\n");
                ammo -= 1;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat est mort.\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case ATTACK:
            
            if (!enemy_spoted) {
                global_current_state = PATROL;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat est en patrouille.\n");
                
                break;
            }
            
            if (ammo <= 0) {
                global_current_state = RELOAD;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat recharge.\n");
                ammo = max_ammo;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat est mort.\n");
                
                break;
            }
            
            if (health < (health/2)) {
                global_current_state = TAKE_COVER;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat se cache.\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case PATROL:
            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat tire.\n");
                ammo -= 1;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat est mort.\n");
                
                break;
            }
            
            break;
        
        case RELOAD:
            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat tire.\n");
                ammo -= 1;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le soldat est mort.\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case DEAD:
            
            break;
        
        case WINNER:
            
            break;
        
        case IDLE:
            
            if (enemy_spotted) {
                global_current_state = PATROL;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Tourelle en patrouille\n");
                
                break;
            }
            
            if (health <= 0 || turret_health <= 0) {
                global_current_state = DEAD;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Tourelle HS !\n");
                
                break;
            }
            
            break;
        
        case PATROL:
            
            if (health <= 0 || turret_health <= 0) {
                global_current_state = DEAD;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Tourelle HS !\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Le joueur est mort.\n");
                
                break;
            }
            
            if (enemy_spotted) {
                global_current_state = PATROL;
                global_state_entered = false; // Reset the state entry flag
                
                printf("Tourelle en patrouille\n");
                
                break;
            }
            
            break;
        
        case WINNER:
            
            break;
        
        case DEAD:
            
            break;
        
    }
}

void* global_run(void* arg) {
    while (true) {
        global_step();
        usleep(100000);
    }
    return NULL;
}

typedef enum {
    
    IDLE,
    
    PATROL,
    
    WINNER,
    
    DEAD,
    
} Turret_state;

static Turret_state Turret_current_state = IDLE;
static bool Turret_state_entered = false; // Tracks if the state was just entered

void Turret_step() {
    if (!Turret_state_entered) {
        switch (Turret_current_state) {
            
            case IDLE:
                
                printf("Tourelle en attente.\n");
                
                Turret_state_entered = true; // Mark the state as entered
                break;
            
            case PATROL:
                
                printf("Tourelle en patrouille\n");
                
                Turret_state_entered = true; // Mark the state as entered
                break;
            
            case WINNER:
                
                printf("Le joueur est mort.\n");
                
                Turret_state_entered = true; // Mark the state as entered
                break;
            
            case DEAD:
                
                printf("Tourelle HS !\n");
                
                Turret_state_entered = true; // Mark the state as entered
                break;
            
        }
    }

    switch (Turret_current_state) {
        
        case IDLE:
            
            break;
        
        case PATROL:
            
            break;
        
        case WINNER:
            
            break;
        
        case DEAD:
            
            break;
        
    }
}

void* Turret_run(void* arg) {
    while (true) {
        Turret_step();
        usleep(100000);
    }
    return NULL;
}


int main() {
    while(1){
        
        global_step();
        
        Turret_step();
        
    }
}