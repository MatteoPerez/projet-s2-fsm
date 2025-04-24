// Preeptive multitasking
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"






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

void Turret_step() {
    while(1){
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

typedef enum {
    
    
                ,
    
    
                ,
    
    SOLITAIRE,
    
} Errors_state;

static Errors_state Errors_current_state = 
                ;

void Errors_step() {
    while(1){
        switch (Errors_current_state) {
        
        case 
                :
            
            No description
            

            
            break;
        
        case 
                :
            
            No description
            

            
            break;
        
        case SOLITAIRE:
            
            No description
            

            
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

void global_step() {
    while(1){
        switch (global_current_state) {
        
        case TAKE_COVER:
            
            printf("Le soldat se cache.\n");
            

            
            if (ammo <= 0) {
                global_current_state = RELOAD;
                
                printf("Le soldat recharge.\n");
                ammo = max_ammo;
                
                break;
            }
            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                
                printf("Le soldat tire.\n");
                ammo -= 1;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                
                printf("Le soldat est mort.\n");
                
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
            ammo -= 1;
            

            
            if (!enemy_spoted) {
                global_current_state = PATROL;
                
                printf("Le soldat est en patrouille.\n");
                
                break;
            }
            
            if (ammo <= 0) {
                global_current_state = RELOAD;
                
                printf("Le soldat recharge.\n");
                ammo = max_ammo;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                
                printf("Le soldat est mort.\n");
                
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
                ammo -= 1;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                
                printf("Le soldat est mort.\n");
                
                break;
            }
            
            break;
        
        case RELOAD:
            
            printf("Le soldat recharge.\n");
            ammo = max_ammo;
            

            
            if (enemy_spotted) {
                global_current_state = ATTACK;
                
                printf("Le soldat tire.\n");
                ammo -= 1;
                
                break;
            }
            
            if (health == 0) {
                global_current_state = DEAD;
                
                printf("Le soldat est mort.\n");
                
                break;
            }
            
            if (player_health <= 0) {
                global_current_state = WINNER;
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case DEAD:
            
            printf("Le soldat est mort.\n");
            

            
            break;
        
        case WINNER:
            
            printf("Le joueur a péri.\n");
            

            
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
    while(1);
}