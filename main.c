/*
* File: ports-io-pwm.c
* Author: jmgonet
*
* Created on February 15, 2014, 2:40 PM
*/
#include <htc.h>

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
* Point d'entrée.
* Configure le port A comme entrée, et le B comme sortie.
* Puis copie la valeur lue du port A dans le port B.
*/
void main()
{
    char sinus[] =
    {
    16, 21, 25, 28, 30, 31, 30,
    28, 25, 21, 16, 11, 7, 4,
    2, 1, 2, 4, 7, 11, 16
    };
    char tmr = 0;
    char pr = 32;
    char ccpr = 0;
    char n;
    
    ANSELB = 0x00; // Désactive les convertisseurs A/D.
    ANSELC = 0x00; // Désactive les convertisseurs A/D.
    
    TRISAbits.RA1 = 1;      // Active RA1 comme entrée.
    ANSELAbits.ANSA1 = 1;   // Active AN1 comme entrée analogique.
    ADCON0bits.ADON = 1;    // Allume le module A/D.
    ADCON0bits.CHS = 1;    // Selection du channel: AN1
    ADCON2bits.ADFM = 0;    // Les 8 bits plus signifiants sur ADRESH.
    ADCON2bits.ACQT = 3; // Temps d'acquisition à 6 TAD.
    ADCON2bits.ADCS = 0; // À 1MHz, le TAD est à 2us.
    ADCON0bits.GO = 1;
    
    TRISC = 0x00; // Tous les bits du port A comme sorties.
    while(1)
    {
        for(n=0; n<20; n++)
        {
            ccpr = sinus[n];
            //pr = ADRESH/8;
            for(tmr=0; tmr<pr; tmr++)
            {
                if (tmr<ccpr)
                {
                    PORTC = 0;
                }
                else
                {
                    PORTC = 4;
                }
            }
        }
    }
}
