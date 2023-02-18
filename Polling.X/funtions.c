#include <xc.h>
#include "config.h"
#include "funtions.h"

/**
 * Se encarga de interpretar el valor obtenido del ADC a BCD
 * @param iADC lectura ADC
 * @return arreglo de 3 elementos que reprecentan el valor
 * BCD de la lectura de entrada, siendo el primer valor
 * el menos significativo o la lectura de 10 mV
 */
unsigned short *BinTOBcd(unsigned long iADC) {

    static unsigned short r[3];
    r[0] = ((iADC * 1000) / 1024) % 10;
    r[1] = (((iADC * 1000) / 1024) / 10) % 10;
    r[2] = (((iADC * 1000) / 1024) / 10) / 10;

    return r;

}

/**
 * Formatea de BCD a 7 segmentos
 * @param iBCD arreglo de tres digitos en BCD a convertir
 * @return arreglo de tres digitos en formato 3 segmento,
 * la posicion inicial del apuntador o [0] es el bit menos
 * significativo
 */
unsigned short *seg7(const unsigned short *iBCD) {

    unsigned short numbers[10] = {
            //ABCDEFG
            0b1111110,      // numero: 0
            0b0110000,      // numere: 1
            0b1101101,      // numero: 2
            0b1111001,      // numero: 3
            0b0110011,      // numero: 4
            0b1011011,      // numero: 5
            0b1011111,      // numero: 6
            0b1110000,      // numero: 7
            0b1111111,      // numero: 8
            0b1111011,      // numero: 9
    };

    static unsigned short r[3];
    r[0] = numbers[iBCD[0]];
    r[1] = numbers[iBCD[1]];
    r[2] = numbers[iBCD[2]];

    return r;

}

/**
 * Configura la salida logica de los pines conectados al display
 * @param digit representacion bit a bit del estado logico necesario
 * para la reprecentacion decimal en el display
 */
void showNumber(unsigned short digit) {
    ADis = !!(digit & (1 << 6));
    BDis = !!(digit & (1 << 5));
    CDis = !!(digit & (1 << 4));
    DDis = !!(digit & (1 << 3));
    EDis = !!(digit & (1 << 2));
    FDis = !!(digit & (1 << 1));
    GDis = digit & 1;
}

/**
 * Realiza un barrido a travez de los digitos del display mostrando
 * de manera individual cada digito, mediante Persistence of Vision
 * se da la ilucion de tener encendido al tiempo cada digito en
 * el display, adicinalmente se configura la frecuencia en el
 * archivo config.h
 * @param digits arreglo de tres elementos que contienen la representacion
 * logica para visualizar correctamente los digitos en el display
 */
void showNumbers(unsigned short *digits) {
    for (int i = 0; i < 10; i++) {
        showNumber(digits[0]);
        T3 = 1;
        __delay_ms(DELAY_SYS);
        T3 = 0;

        showNumber(digits[1]);
        T2 = 1;
        __delay_ms(DELAY_SYS);
        T2 = 0;

        showNumber(digits[2]);
        T1 = 1;
        __delay_ms(DELAY_SYS);
        T1 = 0;
    }
}


/**
 * Comienza la converción del ADC, al final del proceso se desactiva
 * @return la lectura obtenida del ADC
 */
unsigned long readADC(void) {
    ADCON0bits.ADON = 1;                //Habilitamos el ADC

    ADCON0bits.GO_nDONE = 1;            //Iniciamos la convercion

    while (ADCON0bits.GO_nDONE);        //Esperamos a que termine la convercion

    unsigned long r = (unsigned long)((ADRESH << 8) + ADRESL);    //Concatenamos los valores de los registros
    ADCON0bits.ADON = 0;                //Desabilitamos el ADC
    return r;

}

/**
 * Inicializa los registros necesarios para la tranmicion serial UART
 */
void init_UART(void) {
    APFCON1bits.TXCKSEL = 0;        //Seleccionamos el pin RB2 como TX
    TXSTAbits.TRMT = 1;             //Buffer de trasmicion vacio
    TXSTAbits.TXEN = 1;             //Tramicion habilitada
    BAUDCONbits.RCIDL = 1;          //ADC no ocupado


    TXSTAbits.SYNC = 0;             //Transmicion Asincrona
    TXSTAbits.TX9 = 0;              //8 bits de trasmicion
    TXSTAbits.BRGH = 1;             //Velocidad de convercion alta
    SPBRG = 25;                     //Transmicion a 9600 baudios
    RCSTAbits.SPEN = 1;             //No entendi que hace, pero se debe poner

}

/**
 * Trasmite un caracter por el pin TX
 * @param c caracter a trasmitir
 */
void UART_write(unsigned char c) {
    TXREG = c;
    while (TXSTAbits.TRMT == 0);
}

/**
 * Realiza la trasmicion de una cadena de caracteres
 * @param cadena cadena de caracteres a transmitir
 */
void UART_print(unsigned char *cadena) {
    while (*cadena != 0) {
        UART_write(*cadena);
        cadena++;
    }
}

/**
 * Genera una cadena de caracteres
 * @param a Digito mas significativo
 * @param b Digito del medio
 * @param c DIgito menos significativo
 * @return cadena de caracteres de entrada formateada para
 * la trasmicion serie
 */
unsigned char *ASCII_Con(unsigned short a,unsigned short b,unsigned short c) {
    static unsigned char r[7];
    r[0] = (char)(a + 48);
    r[1] = '.';
    r[2] = (char)(b + 48);
    r[3] = (char)(c + 48);;
    r[4] = ' ';
    r[5] = 'V';
    r[6] = '\0';

    return r;
}
