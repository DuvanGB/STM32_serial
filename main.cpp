#include "stm32f7xx.h"
#include <stdio.h>
#include "math.h"
#include <cmath>
#include <stdlib.h>
#define PROMEDIO 10

/*
export default {
  primary: "#16504B",
  spotify: "##0E817B",
  blue: "#1E3A5F",
  green: "#0B4B23",
  brown: "#8B492E",
  redWine: "#3D0A10",
  pink: "#FD0C38",
};
*/

//RESULTADO SENSORES
int valorUART1[3] = {0, 0, 0};
int valorUART2[2] = {0, 0};
int valorUART3[2] = {0, 0};

// VOLTAJE LM35
int valorUART4[3] = {0, 0, 0, 0};


////   ADC1 
float adc_1VOL;
int adc_1TEMP;
////   ADC2
int adc_2RES;
int adc_2LUZ;
////   ADC3
float adc_3VOL;
int adc_3DIS;
////   UART1
int UART_1VOL[10] = {};
int UART_1TEMP[10] = {};
////    UART2
int UART_2RES[10] = {};
int UART_2LUZ[10] = {};
////    UART3
float UART_3VOL[10] = {};
int UART_3DIS[10] = {};
	
int contador;
int resultadoPromedioADC1[2] = {0};
int resultadoPromedioADC2[2] = {0};
int resultadoPromedioADC3[2] = {0};
int auxContadorADC1, auxContadorADC2, auxContadorADC3;
int sumarValoresADC1 = 0, sumarValoresADC2 = 0, sumarValoresADC3 = 0;
int promedioADC1 = 0, promedioADC2 = 0, promedioADC3 = 0;

float maximo_1, minimo_1, porcentaje_1;
float maximo_2, minimo_2, porcentaje_2;
float maximo_3, minimo_3, porcentaje_3;

////////////////////OTRAS FUNCIONES////////////////////
//CALCULAR PROMEDIOS DE LOS ADCS//
void promedioSensor1(){ //PROMEDIO PARA EL SENSOR LM35
	for(int i = 0; i < PROMEDIO; i++){
		sumarValoresADC1 = sumarValoresADC1 + UART_1TEMP[i];
	}
	promedioADC1 = sumarValoresADC1 / PROMEDIO;
	resultadoPromedioADC1[auxContadorADC1] = promedioADC1;
	auxContadorADC1++;
	if(auxContadorADC1 > 1){auxContadorADC1 = 0;}
}

void promedioSensor2(){ //PROMEDIO PARA EL SENSOR FOTOCELDA
	for(int i = 0; i < PROMEDIO; i++){
		sumarValoresADC2 = sumarValoresADC2 + UART_2LUZ[i];
	}
	promedioADC2 = sumarValoresADC2 / PROMEDIO;
	resultadoPromedioADC2[auxContadorADC2] = promedioADC2;
	auxContadorADC2++;
	if(auxContadorADC2 > 1){auxContadorADC2 = 0;}
}

void promedioSensor3(){ //PROMEDIO PARA EL SENSOR SHARP
	for(int i = 0; i < PROMEDIO; i++){
		sumarValoresADC3 = sumarValoresADC3 + UART_3DIS[i];
	}
	promedioADC3 = sumarValoresADC3 / PROMEDIO;
	resultadoPromedioADC3[auxContadorADC3] = promedioADC3;
	auxContadorADC3++;
	if(auxContadorADC3 > 1){auxContadorADC3 = 0;}
}

//ALARMA SENSOR LM35
void alarmaSensor1(){
	porcentaje_1 = resultadoPromedioADC1[0] * 0.1; 
	maximo_1 = resultadoPromedioADC1[0] + porcentaje_1;
	minimo_1 = resultadoPromedioADC1[0] - porcentaje_1;
	if(maximo_1 < resultadoPromedioADC1[1] || minimo_1 > resultadoPromedioADC1[1]){
			GPIOB -> ODR ^= (1UL << 0);
		for(int i = 0; i < 100000; i++);
	}else{
		GPIOB -> ODR = 0;
	}
}

//ALARMA SENSOR FOTOCELDA
void alarmaSensor2(){
	porcentaje_2 = resultadoPromedioADC2[0] * 0.1;
	maximo_2 = resultadoPromedioADC2[0] + porcentaje_2;
	minimo_2 = resultadoPromedioADC2[0] - porcentaje_2;
	if(maximo_2 < resultadoPromedioADC2[1] || minimo_2 > resultadoPromedioADC2[1]){
			GPIOB -> ODR ^= (1UL << 1);
		for(int i = 0; i < 100000; i++);
	}else{
		GPIOB -> ODR = 0;
	}
}

//ALARMA SENSOR SHARP
void alarmaSensor3(){
	porcentaje_3 = resultadoPromedioADC3[0] * 0.1;
	maximo_3 = resultadoPromedioADC3[0] + porcentaje_3;
	minimo_3 = resultadoPromedioADC3[0] - porcentaje_3;
	if(maximo_3 < resultadoPromedioADC3[1] || minimo_3 > resultadoPromedioADC3[1]){
			GPIOB -> ODR ^= (1UL << 2);
		for(int i = 0; i < 100000; i++);
	}else{
		GPIOB -> ODR = 0;
	}
}

//VALORES DEL SENSOR LM35
void descomponerUART1(int valor1){ //DESCOMPONER TEMPERATURAADC1
	valorUART1[0] = valor1 / 100;
	valorUART1[1] = (valor1 % 100) / 10;
	valorUART1[2] = (valor1 % 10);
}
void descomponerUART4(int valor4){ //DESCOMPONER TEMPERATURAADC1
	valorUART4[0] = valor4 / 1000;
	valorUART4[1] = (valor4 % 1000) / 100;
	valorUART4[2] = (valor4 % 100) / 10;
	valorUART4[3] = (valor4 % 10);
}

//VALORES DEL SENSOR FOTORESISTENCIA
void descomponerUART2(int valor2){ //DESCOMPONER DECIMALES ADC2
	valorUART2[0] = (valor2 % 100) / 10;
	valorUART2[1] = (valor2 % 10);
}

//VALORES DEL SENSOR SHARP
void descomponerUART3(int valor3){ //DESCOMPONER DECIMALES ADC3
	valorUART3[0] = (valor3 % 100) / 10;
	valorUART3[1] = (valor3 % 10);
}

/////////////FUNCION PRINCIPAL//////////////
int main(void){
	RCC -> AHB1ENR |= 0X16;	 //PUERTOS C, E, B
	RCC -> APB1ENR |= 0X80000; //HABILITAR UART4
	RCC -> APB2ENR |= 0X100; // ADC1
	RCC -> APB2ENR |= 0X200; // ADC2
	RCC -> APB2ENR |= 0X400; // ADC3
	
	GPIOB -> MODER = 0X5555; //LEDS ALARMA
	GPIOB -> OTYPER = 0X0;
	GPIOB -> OSPEEDR = 0X5555;
	GPIOB -> PUPDR = 0X5555;
	
	GPIOE -> MODER = 0X555555; //LCD
	GPIOE -> OTYPER = 0X0;
	GPIOE -> OSPEEDR = 0X555555;
	GPIOE -> PUPDR = 0X555555;
	
	GPIOC -> MODER |= 0XA0003F; // MODO ALTERNANTE(UART) Y MODO ANALOGO PUERTO C (PIN 0 - PIN 1 - PIN 3)
	GPIOC -> AFR[1] |= 0X8800;  // CONFIGURACION DEL REGISTRO ALTERNO PARA EL TX Y RX

	//UART
	UART4 -> CR1 |= 0X2D;	 // HABITILITA UART, TRANSMISOR, RECEPTOR Y LA INTERRUPCION POR RECEPCION
	UART4 -> BRR |= 0X683; //9600 BAUDIOS

	//ADCS
	ADC1 -> CR1 |= 0X0; //RESOLUCION DE 12 BITS
	ADC1 -> CR2 |= 0X401;		//ENCENDER EL ADC (END OF CONVERSION, HABILITAR ADC)
	ADC1 -> SQR3 = 10;	// CANAL 10 DEL ADC
	
	ADC2 -> CR1 |= 0X0;	//RESOLUCION DE 12 BITS (POR DEFECTO)
	ADC2 -> CR2 |= 0X401; ////ENCENDER EL ADC (END OF CONVERSION, HABILITAR ADC)
	ADC2 -> SQR3 = 11; // CANAL 11 DEL ADC(ADC2, PINC1)
	
	ADC3 -> CR1 |= 0X0;	//RESOLUCION DE 12 BITS (POR DEFECTO)
	ADC3 -> CR2 |= 0X401; ////ENCENDER EL ADC (END OF CONVERSION, HABILITAR ADC)
	ADC3 -> SQR3 = 12; // CANAL 12 DEL ADC(ADC3, PINC2)
	
	while(true){
		ADC1->CR2 |= 0X40000000; // INICIAR LA CONVERSION EN CANAL REGULARES
		ADC2->CR2 |= 0X40000000; // INICIAR LA CONVERSION EN CANAL REGULARES
		ADC3->CR2 |= 0X40000000; // INICIAR LA CONVERSION EN CANAL REGULARES
		
		while ((ADC1 -> SR &= 0X2) == 1);  //LM35 DE 0 A 150 °C
		adc_1VOL = ((ADC1 -> DR) * 1.5 / 4095) * 1000; 	
		adc_1TEMP = ((ADC1 -> DR) * 1.5 / 4095) * 100; 	
		UART_1VOL[contador] = adc_1VOL; 
		UART_1TEMP[contador] = adc_1TEMP;
		
		while ((ADC2 -> SR &= 0X2) == 1); //FOTOCELDA 
//		adc_2RES = (4925 / ((ADC2 -> DR) * 4.0 / 4095)) - 985;
		adc_2LUZ = ((ADC2 -> DR)*5000)/(50 * (4095-(ADC2 -> DR)));
//		UART_2RES[contador] = adc_2RES;
		UART_2LUZ[contador] = adc_2LUZ; 
		
		while ((ADC3 -> SR &= 0X2) == 1); //SHARP
//		adc_3VOL = (ADC3 -> DR) * 5.0 / 4095; 
		adc_3DIS = 17569.7 * (pow((ADC3 -> DR), -1.2062)); 
//		UART_3VOL[contador] = adc_3VOL;
		UART_3DIS[contador] = adc_3DIS; 
		
		contador++;
		if(contador > 9){
			promedioSensor1();
			alarmaSensor1();
			for(int i = 0; i < 10000; i++);
			promedioSensor2();
			alarmaSensor2();
			for(int i = 0; i < 10000; i++);
			promedioSensor3();
			alarmaSensor3();
			
			//ENVIAR DATOS DEL SENSOR LM35 UART TEMPERATURA
			for(int i = 0; i < 10; i++){
			descomponerUART1(UART_1TEMP[i]);
			descomponerUART4(UART_1VOL[i]);
				for(int e = 0; e < 3; e++){
					UART4 -> TDR = valorUART1[e] + 48;
					while ((UART4->ISR &= 0X80) == 0);
				}			
			}

//			//ENVIAR DATOS DEL SENSOR FOTORESISTENCIA UART LUZ		
			for(int i = 0; i < 10; i++){
			descomponerUART2(UART_2LUZ[i]);
				for(int e = 0; e < 2; e++){
					UART4 -> TDR = valorUART2[e] + 48;
					while ((UART4->ISR &= 0X80) == 0);
				}
			}

//			//ENVIAR DATOS DEL SENSOR SHARP UART DISTANCIA		
			for(int i = 0; i < 10; i++){
			descomponerUART3(UART_3DIS[i]);
				for(int e = 0; e < 2; e++){
					UART4 -> TDR = valorUART3[e] + 48;
					while ((UART4->ISR &= 0X80) == 0);
				}
			}
			
			for(int i = 0; i < 10; i++){
			descomponerUART4(UART_1VOL[i]);
				for(int e = 0; e < 4; e++){
					UART4 -> TDR = valorUART4[e] + 48;
					while ((UART4->ISR &= 0X80) == 0);
				}
			}
			
		}else{
				promedioADC1 = 0, sumarValoresADC1 = 0;
				promedioADC2 = 0, sumarValoresADC2 = 0;
				promedioADC3 = 0, sumarValoresADC3 = 0;
		}		
		
		for(int i = 0; i < 200000; i++);
		if(contador > (PROMEDIO - 1)){contador = 0;}
	}
}