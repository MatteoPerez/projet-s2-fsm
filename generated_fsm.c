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
    
} global_State;

static global_State global_current_state = PATROL;

void global_step() {
    switch (global_current_state) {
        
        case TAKE_COVER:
            
            printf("Le soldat se met se cache.\n");
            

            
            if (/* condition pour transition */) {
                global_current_state = RELOAD;
                
                printf("Le soldat recharge.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = ATTACK;
                
                printf("Le soldat tire.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = DEAD;
                
                printf("Le soldat est morti.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = WINNER;
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case ATTACK:
            
            printf("Le soldat tire.\n");
            

            
            if (/* condition pour transition */) {
                global_current_state = PATROL;
                
                printf("Le soldat est en patrouille.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = RELOAD;
                
                printf("Le soldat recharge.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = DEAD;
                
                printf("Le soldat est morti.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = TAKE_COVER;
                
                printf("Le soldat se met se cache.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = WINNER;
                
                printf("Le joueur a péri.\n");
                
                break;
            }
            
            break;
        
        case PATROL:
            
            printf("Le soldat est en patrouille.\n");
            

            
            if (/* condition pour transition */) {
                global_current_state = ATTACK;
                
                printf("Le soldat tire.\n");
                
                break;
            }
            
            if (/* condition pour No more health */) {
                global_current_state = DEAD;
                
                printf("Le soldat est morti.\n");
                
                break;
            }
            
            break;
        
        case RELOAD:
            
            printf("Le soldat recharge.\n");
            

            
            if (/* condition pour transition */) {
                global_current_state = TAKE_COVER;
                
                printf("Le soldat se met se cache.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = ATTACK;
                
                printf("Le soldat tire.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = DEAD;
                
                printf("Le soldat est morti.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
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
            

            
            if (/* condition pour transition */) {
                global_current_state = PATROL;
                
                printf("Tourelle en patrouille\n");
                
                break;
            }
            
            if (/* condition pour No more health (player or turret) */) {
                global_current_state = DEAD;
                
                printf("Tourelle HS !\n");
                
                break;
            }
            
            break;
        
        case PATROL:
            
            printf("Tourelle en patrouille\n");
            

            
            if (/* condition pour transition */) {
                global_current_state = DEAD;
                
                printf("Tourelle HS !\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = WINNER;
                
                printf("Le joueur est morti.\n");
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = PATROL;
                
                printf("Tourelle en patrouille\n");
                
                break;
            }
            
            break;
        
        case WINNER:
            
            printf("Le joueur est morti.\n");
            

            
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
    
} Group_1_State;

static Group_1_State Group_1_current_state = IDLE;

void Group_1_step() {
    switch (Group_1_current_state) {
        
        case IDLE:
            
            printf("Tourelle en attente.\n");
            

            
            break;
        
        case PATROL:
            
            printf("Tourelle en patrouille\n");
            

            
            break;
        
        case WINNER:
            
            printf("Le joueur est morti.\n");
            

            
            break;
        
        case DEAD:
            
            printf("Tourelle HS !\n");
            

            
            break;
        
    }
}

void* Group_1_run(void* arg) {
    while (true) {
        Group_1_step();
        usleep(100000);
    }
    return NULL;
}



int main() {
    pthread_t threads[2];
    int thread_index = 0;

    
    pthread_create(&threads[thread_index++], NULL, global_run, NULL);
    
    pthread_create(&threads[thread_index++], NULL, Group_1_run, NULL);
    

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}