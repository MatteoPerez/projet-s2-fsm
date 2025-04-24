// Preemptive multitasking (pthread)
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>






int max_ammo = 5;
int ammo = max_ammo;
int health = 6;



typedef enum {
    
    IDLE,
    
    PATROL,
    
    WINNER,
    
    DEAD,
    
} Turret_state;

static Turret_state Turret_current_state = IDLE;
static bool Turret_state_entered = false;

void Turret_step() {
    if (!Turret_state_entered) {
        switch (Turret_current_state) {
            
            case IDLE:
                
                printf("Tourelle en attente.\n");
                
                Turret_state_entered = true;
                break;
            
            case PATROL:
                
                printf("Tourelle en patrouille\n");
                
                Turret_state_entered = true;
                break;
            
            case WINNER:
                
                printf("Le joueur est mort.\n");
                
                Turret_state_entered = true;
                break;
            
            case DEAD:
                
                printf("Tourelle HS !\n");
                
                Turret_state_entered = true;
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

typedef enum {
    
    
                ,
    
    
                ,
    
    SOLITAIRE,
    
} Errors_state;

static Errors_state Errors_current_state = 
                ;
static bool Errors_state_entered = false;

void Errors_step() {
    if (!Errors_state_entered) {
        switch (Errors_current_state) {
            
            case 
                :
                
                No description
                
                Errors_state_entered = true;
                break;
            
            case 
                :
                
                No description
                
                Errors_state_entered = true;
                break;
            
            case SOLITAIRE:
                
                No description
                
                Errors_state_entered = true;
                break;
            
        }
    }

    switch (Errors_current_state) {
        
        case 
                :
            
            break;
        
        case 
                :
            
            break;
        
        case SOLITAIRE:
            
            break;
        
    }
}

void* Errors_run(void* arg) {
    while (true) {
        Errors_step();
        usleep(100000);
    }
    return NULL;
}

typedef enum {
    
    TAKE_COVER,
    
    ATTACK,
    
    PATROL,
    
    RELOAD,
    
    DEAD,
    
    WINNER,
    
} global_state;

static global_state global_current_state = PATROL;
static bool global_state_entered = false;

void global_step() {
    if (!global_state_entered) {
        switch (global_current_state) {
            
            case TAKE_COVER:
                
                printf("Le soldat se cache.\n");
                
                global_state_entered = true;
                break;
            
            case ATTACK:
                
                printf("Le soldat tire.\n");
                ammo -= 1;
                
                global_state_entered = true;
                break;
            
            case PATROL:
                
                printf("Le soldat est en patrouille.\n");
                
                global_state_entered = true;
                break;
            
            case RELOAD:
                
                printf("Le soldat recharge.\n");
                ammo = max_ammo;
                
                global_state_entered = true;
                break;
            
            case DEAD:
                
                printf("Le soldat est mort.\n");
                
                global_state_entered = true;
                break;
            
            case WINNER:
                
                printf("Le joueur a péri.\n");
                
                global_state_entered = true;
                break;
            
        }
    }

    switch (global_current_state) {
        
        case TAKE_COVER:
            
            if (ammo <= 0) {
                global_current_state = RELOAD;
                global_state_entered = false;
                
                printf("Le soldat recharge.\n");
                ammo = max_ammo;
                
                break;
            }
            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                global_state_entered = false;
                
                printf("Le soldat tire.\n");
                ammo -= 1;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                global_state_entered = false;
                
                printf("Le soldat est mort.\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                global_state_entered = false;
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case ATTACK:
            
            if (!enemy_spoted) {
                global_current_state = PATROL;
                global_state_entered = false;
                
                printf("Le soldat est en patrouille.\n");
                
                break;
            }
            
            if (ammo <= 0) {
                global_current_state = RELOAD;
                global_state_entered = false;
                
                printf("Le soldat recharge.\n");
                ammo = max_ammo;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                global_state_entered = false;
                
                printf("Le soldat est mort.\n");
                
                break;
            }
            
            if (health < (health/2)) {
                global_current_state = TAKE_COVER;
                global_state_entered = false;
                
                printf("Le soldat se cache.\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                global_state_entered = false;
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case PATROL:
            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                global_state_entered = false;
                
                printf("Le soldat tire.\n");
                ammo -= 1;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                global_state_entered = false;
                
                printf("Le soldat est mort.\n");
                
                break;
            }
            
            break;
        
        case RELOAD:
            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                global_state_entered = false;
                
                printf("Le soldat tire.\n");
                ammo -= 1;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                global_state_entered = false;
                
                printf("Le soldat est mort.\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                global_state_entered = false;
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case DEAD:
            
            break;
        
        case WINNER:
            
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


int main() {
    pthread_t threads[3];
    int thread_index = 0;

    
    pthread_create(&threads[thread_index++], NULL, Turret_run, NULL);
    
    pthread_create(&threads[thread_index++], NULL, Errors_run, NULL);
    
    pthread_create(&threads[thread_index++], NULL, global_run, NULL);
    

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}