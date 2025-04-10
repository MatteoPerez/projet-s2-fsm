typedef enum {
    STATE_TAKE_COVER,
    STATE_ATTACK,
    STATE_PATROL,
    STATE_RELOAD,
    STATE_DEAD,
    STATE_WINNER,
    
} State;

State current_state = STATE_TAKE_COVER;

void fsm_transition(const char* event) {
    switch (current_state) {
        
        case STATE_TAKE_COVER:
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_RELOAD;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_ATTACK;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_DEAD;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_WINNER;
            }
            
            break;
        
        case STATE_ATTACK:
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_PATROL;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_RELOAD;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_DEAD;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_TAKE_COVER;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_WINNER;
            }
            
            break;
        
        case STATE_PATROL:
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_ATTACK;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_DEAD;
            }
            
            break;
        
        case STATE_RELOAD:
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_TAKE_COVER;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_ATTACK;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_DEAD;
            }
            
            if (strcmp(event, "None") == 0) {
                
                current_state = STATE_WINNER;
            }
            
            break;
        
        case STATE_DEAD:
            
            break;
        
        case STATE_WINNER:
            
            break;
        
    }
}