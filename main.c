/*
* File: ports-io-pwm.c
* Author: jmgonet
* Modified by: Alain Wermelinger
* Created on February 15, 2014, 2:40 PM
*/
#include <htc.h>
#include "signal.h"

/**
* Bits de configuration:
*/
#pragma config FOSC = INTIO67 // Osc. interne, A6 et A7 comme IO.
#pragma config IESO = OFF // Pas d'osc. au démarrage.
#pragma config FCMEN = OFF // Pas de monitorage de l'oscillateur.
// Nécessaires pour ICSP / ICD:
#pragma config MCLRE = EXTMCLR // RE3 est actif comme master reset.
#pragma config WDTEN = OFF // Watchdog inactif.
#pragma config LVP = OFF // Single Supply Enable bits off.

/**
 * Initialise le hardware.
 */
static void hardwareInitialise() {
    // À implémenter.
    /*Configuration Hardware*/   
                                        
    ANSELB = 0x00; // Désactive les convertisseurs A/D.
    ANSELC = 0x00; // Désactive les convertisseurs A/D.
    
    TRISAbits.RA1 = 1;      // Active RA1 comme entrée.
    ANSELAbits.ANSA1 = 1;   // Active AN1 comme entrée analogique.
    ADCON0bits.ADON = 1;    // Allume le module A/D.
    ADCON0bits.CHS = 1;    // Selection du channel: AN1
    ADCON2bits.ADFM = 0;    // Les 8 bits plus signifiants sur ADRESH.
    ADCON2bits.ACQT = 3; // Temps d'acquisition à 6 TAD.
    ADCON2bits.ADCS = 0; // À 1MHz, le TAD est à 2us.
    
    TRISC = 0x00; // Tous les bits du port A comme sorties.
    
    //Temporisateur 1
    // Diviseur de fréquence à 1:
    T1CONbits.T1CKPS = 0;       // Diviseur de fréquence inactif.
    T1CONbits.TMR1ON = 1;       // Active le temporisateur 2 
    // À chaque interruption du timer1:
    TMR1H = 0xCF;
    TMR1L = 0x2C;
    
    // Temporisateur 2 
    T2CONbits.T2CKPS = 0;       // Pas de diviseur de fréquence.
    T2CONbits.TMR2ON = 1;       // Active le temporisateur 2    
    PR2 = 100;                  // Compte jusqu'à 100 (100*4us=400us)
    CCPTMRS0bits.C1TSEL = 00;           // CCP1 utilise TMR2
    CCP1CONbits.CCP1M = 0b00001111;     // PWM mode

    // Active les interruptions de haute et de basse priorité:
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    PIE1bits.ADIE = 1;          // Active les interruptions de conv AD
    PIE1bits.TMR1IE = 1;        // Active les interruption du TMR1    
    PIE1bits.TMR2IE = 1;        // Active les interruption du TMR2   
    IPR1bits.ADIP = 1;
}
/**
 * Point d'entrée des interruptions.
 */
void low_priority interrupt interruptionsBassePriorite() {
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;
        ADCON0bits.GO = 1;
        // À chaque interruption du timer1:
        TMR1H = 0xCF;
        TMR1L = 0x2C;
    }
    
    if (PIR1bits.TMR2IF) {
        PIR1bits.TMR2IF = 0;
        CCPR1L = echantillon();
    }
    
    if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;
        selectionneTableau(ADRESL>>6);
    }
}

/**
* Point d'entrée.
*/
void main()
{
    hardwareInitialise();
    while(1);  
}
