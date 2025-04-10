#include <stdio.h>
#include <stdbool.h>


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

static global_State global_current_state = TAKE_COVER;


typedef enum {
    
    IDLE,
    
    PATROL,
    
    WINNER,
    
    DEAD,
    
} Group 1_State;

static Group 1_State Group 1_current_state = IDLE;




void global_step();

void Group 1_step();



void global_step() {
    switch (global_current_state) {
        
        case TAKE_COVER:
            
            

            
            if (/* condition pour transition */) {
                global_current_state = RELOAD;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = ATTACK;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = DEAD;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = WINNER;
                
                break;
            }
            
            break;
        
        case ATTACK:
            
            

            
            if (/* condition pour transition */) {
                global_current_state = PATROL;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = RELOAD;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = DEAD;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = TAKE_COVER;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = WINNER;
                
                break;
            }
            
            break;
        
        case PATROL:
            
            

            
            if (/* condition pour transition */) {
                global_current_state = ATTACK;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = DEAD;
                
                break;
            }
            
            break;
        
        case RELOAD:
            
            

            
            if (/* condition pour transition */) {
                global_current_state = TAKE_COVER;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = ATTACK;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = DEAD;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = WINNER;
                
                break;
            }
            
            break;
        
        case DEAD:
            
            

            
            break;
        
        case WINNER:
            
            

            
            break;
        
        case IDLE:
            
            

            
            if (/* condition pour transition */) {
                global_current_state = PATROL;
                
                break;
            }
            
            break;
        
        case PATROL:
            
            

            
            if (/* condition pour transition */) {
                global_current_state = DEAD;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = WINNER;
                
                break;
            }
            
            if (/* condition pour transition */) {
                global_current_state = PATROL;
                
                break;
            }
            
            break;
        
        case WINNER:
            
            

            
            break;
        
        case DEAD:
            
            

            
            break;
        
    }
}

void Group 1_step() {
    switch (Group 1_current_state) {
        
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


int main() {
    while (true) {
        
        global_step();
        
        Group 1_step();
        
    }

    return 0;
}