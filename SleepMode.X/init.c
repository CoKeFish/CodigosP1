
#include "init.h"

void init_IO(void)
{
        //Configuracion oscilador
    OSCCONbits.SCS = 0;             //Elegimos el oscilador interno
    OSCCONbits.IRCF = 0b1101;       //4Mhz frecuencia oscilador
    
    //Configuracion puertos
    
    //Configuramos que pines son entradas analogicas del puerto A
    //!!Debe configurarse el correspondiente TRIS
    ANSELAbits.ANSA0 = 1;       //Asignamos el pin RA0 como entrada analogica 
    ANSELAbits.ANSA1 = 0;
    ANSELAbits.ANSA2 = 0;
    ANSELAbits.ANSA3 = 0;
    ANSELAbits.ANSA4 = 0;
    
    //Configuramos las entradas y salidas del puerto A
    TRISAbits.TRISA0 = 1;       //Configuramos el pin RA0 como entrada para ADC
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA6 = 0;
    TRISAbits.TRISA7 = 0;
    
    //Configuramos que pines son entradas analogicas del puerto B
    //!!Debe configurarse el correspondiente TRIS
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;
    ANSELBbits.ANSB4 = 0;
    ANSELBbits.ANSB5 = 0;
    ANSELBbits.ANSB6 = 0;
    ANSELBbits.ANSB7 = 0;
    
    //Configuramos las entradas y salidas del puerto B
    TRISBbits.TRISB0 = 0;
    RX = 1;                     //Configuramos el PIN receptor como entrada
    TX = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;
    
    //ADC
    ADCON0bits.ADON = 0;
    FVRCONbits.FVREN = 1;       //Habilitamos el voltaje de referencia interno
    FVRCONbits.ADFVR = 0b11;    //Asignamos el voltaje de referencia a 4.096V

    ADCON1bits.ADCS = 0b001;    //Seleccionamos el reloj de convercion
    ADCON0bits.CHS = 0b00000;   //Seleccionamos el canal
    ADCON1bits.ADFM = 1;        //Queremos el resultado con justificacion derecha
    ADCON1bits.ADNREF = 0;      //Asignamos el voltaje de referencia negativo a GND
    ADCON1bits.ADPREF = 0b11;   //Asignamos el voltaje de referencia positivo al voltaje de referencia interno

    PIE1bits.ADIE = 1;          //Habilitamos las interrupciones del ADC
    PIR1bits.ADIF = 0;          //Nos aseguramos que este en 0 la bandera
    
    
    //Wachdog timer
    WDTCONbits.WDTPS = 0b01010;
    
    ADis = 0;
    BDis = 0;
    CDis = 0;
    DDis = 0;
    EDis = 0;
    FDis = 0;
    GDis = 0;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    init_UART();                //Configuramos el UART
}

//Configuramos el UART
void init_UART(void)
{
    APFCON1bits.TXCKSEL = 0;        //Seleccionamos el pin RB2 como TX
    TXSTAbits.TRMT = 1;             //Buffer de trasmicion vacio
    TXSTAbits.TXEN = 1;             //Tramicion habilitada
    BAUDCONbits.RCIDL = 1;          //ADC no ocupado
    
    
    TXSTAbits.SYNC = 0;             //Transmicion Asincrona
    TXSTAbits.TX9 = 0;              //8 bits de trasmicion
    TXSTAbits.BRGH = 1;             //Velocidad de convercion alta
    SPBRG = 25;                     //Transmicion a 9600 baudios
    RCSTAbits.SPEN = 1;             //Configuracion interna de pines
    
}

