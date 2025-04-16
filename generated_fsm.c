// Cooperative multitasking
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
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

void global_step() {
    switch (global_current_state) {
        
        case TAKE_COVER:
            
            printf("Le soldat se cache.\n");
            

            
            if (ammo <= 0) {
                global_current_state = RELOAD;
                
                printf("Le soldat recharge.\n");
                
                break;
            }
            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                
                printf("Le soldat tire.\n");
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                
                printf("Le soldat est morti.\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case ATTACK:
            
            printf("Le soldat tire.\n");
            

            
            if (!enemy_spoted) {
                global_current_state = PATROL;
                
                printf("Le soldat est en patrouille.\n");
                
                break;
            }
            
            if (ammo <= 0) {
                global_current_state = RELOAD;
                
                printf("Le soldat recharge.\n");
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                
                printf("Le soldat est morti.\n");
                
                break;
            }
            
            if (health < (health/2)) {
                global_current_state = TAKE_COVER;
                
                printf("Le soldat se cache.\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case PATROL:
            
            printf("Le soldat est en patrouille.\n");
            

            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                
                printf("Le soldat tire.\n");
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                
                printf("Le soldat est morti.\n");
                
                break;
            }
            
            break;
        
        case RELOAD:
            
            printf("Le soldat recharge.\n");
            

            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                
                printf("Le soldat tire.\n");
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                
                printf("Le soldat est morti.\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case DEAD:
            
            printf("Le soldat est morti.\n");
            

            
            break;
        
        case WINNER:
            
            printf("Le joueur a péri.\n");
            

            
            break;
        
        case IDLE:
            
            printf("Tourelle en attente.\n");
            

            
            if (enemy_spotted) {
                global_current_state = PATROL;
                
                printf("Tourelle en patrouille\n");
                
                break;
            }
            
            if (health <= 0 || turret_health <= 0) {
                global_current_state = DEAD;
                
                printf("Tourelle HS !\n");
                
                break;
            }
            
            break;
        
        case PATROL:
            
            printf("Tourelle en patrouille\n");
            

            
            if (health <= 0 || turret_health <= 0) {
                global_current_state = DEAD;
                
                printf("Tourelle HS !\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                
                printf("Le joueur est mort.\n");
                
                break;
            }
            
            if (enemy_spotted) {
                global_current_state = PATROL;
                
                printf("Tourelle en patrouille\n");
                
                break;
            }
            
            break;
        
        case WINNER:
            
            printf("Le joueur est mort.\n");
            

            
            break;
        
        case DEAD:
            
            printf("Tourelle HS !\n");
            

            
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

void Turret_step() {
    switch (Turret_current_state) {
        
        case IDLE:
            
            printf("Tourelle en attente.\n");
            

            
            break;
        
        case PATROL:
            
            printf("Tourelle en patrouille\n");
            

            
            break;
        
        case WINNER:
            
            printf("Le joueur est mort.\n");
            

            
            break;
        
        case DEAD:
            
            printf("Tourelle HS !\n");
            

            
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