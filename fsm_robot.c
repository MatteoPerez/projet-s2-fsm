#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

// Simule un événement partagé entre FSM
volatile bool obstacle_detecte = false;

// FSM Mouvement
typedef enum { AVANCER, TOURNER } EtatMouvement;
EtatMouvement etat_mouvement = AVANCER;

void* fsm_mouvement(void* arg) {
    while (1) {
        switch (etat_mouvement) {
            case AVANCER:
                printf("[MOUVEMENT] Avancer...\n");
                if (obstacle_detecte) {
                    etat_mouvement = TOURNER;
                }
                break;
            case TOURNER:
                printf("[MOUVEMENT] Tourner pour éviter l'obstacle.\n");
                sleep(1);
                obstacle_detecte = false;
                etat_mouvement = AVANCER;
                break;
        }
        sleep(1);
    }
    return NULL;
}

// FSM Capteur
typedef enum { VEILLE, ALERTE } EtatCapteur;
EtatCapteur etat_capteur = VEILLE;

void* fsm_capteur(void* arg) {
    while (1) {
        switch (etat_capteur) {
            case VEILLE:
                if (rand() % 10 > 7) {
                    printf("[CAPTEUR] Obstacle détecté !\n");
                    obstacle_detecte = true;
                    etat_capteur = ALERTE;
                } else {
                    printf("[CAPTEUR] Rien à signaler.\n");
                }
                break;
            case ALERTE:
                if (!obstacle_detecte) {
                    etat_capteur = VEILLE;
                }
                break;
        }
        sleep(1);
    }
    return NULL;
}

// FSM Communication
typedef enum { IDLE, TRAITER_MSG } EtatComm;
EtatComm etat_comm = IDLE;

void* fsm_communication(void* arg) {
    while (1) {
        switch (etat_comm) {
            case IDLE:
                if (rand() % 10 > 8) {
                    printf("[COMM] Message reçu !\n");
                    etat_comm = TRAITER_MSG;
                } else {
                    printf("[COMM] En attente de message...\n");
                }
                break;
            case TRAITER_MSG:
                printf("[COMM] Traitement du message...\n");
                sleep(2);
                etat_comm = IDLE;
                break;
        }
        sleep(1);
    }
    return NULL;
}

// Main
int main() {
    srand(time(NULL));

    pthread_t thread_mouv, thread_capt, thread_comm;

    pthread_create(&thread_mouv, NULL, fsm_mouvement, NULL);
    pthread_create(&thread_capt, NULL, fsm_capteur, NULL);
    pthread_create(&thread_comm, NULL, fsm_communication, NULL);

    pthread_join(thread_mouv, NULL);
    pthread_join(thread_capt, NULL);
    pthread_join(thread_comm, NULL);

    return 0;
}
