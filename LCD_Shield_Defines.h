/*
*En este header crearé los diferentes defines y struct que necesito para la creación de la clase para la Shield
*/


#ifndef INCFILE1_H_
#define INCFILE1_H_

/************************************************************************/
/*				INCLUDE FILES                                           */
/************************************************************************/
#include <LiquidCrystal.h>
#include <Arduino.h>
#include <stdint.h>



/************************************************************************/
/*				Defines for Sensors and Threshold                       */
/************************************************************************/
#define VOLTAGE_DEFAULT			14.5	//Volts
#define	CURRENT_DEFAULT			3.0		//Amperes
#define TIME_MAX_DEFAULT		300		//minutes
#define TIME_MAX_COMPARING		20.0	//minutes

#define CURRENT_SENSIBILITY		0.185	//
#define MEASURE_SAMPLES			500

#define BOTONS_PINS				A0
#define CURRENT_SENSOR_PIN		A4
#define VOLTAGE_SENSOR_PIN		A5
#define RELE_PIN				12

#define TIMER2_SECOND_COUNTER	1838

#define SPANISH_LANGUAGE
//#define ENGLISH_LANGUAGE
/************************************************************************/
/*				CREACIÓN DE STRUCTS PARA LOS SENSORES                   */
/************************************************************************/
typedef struct{
	float		VoltageSensor;
	float		CurrentSensor;
	uint16_t	TimerCounter;
	float		VoltageThreshold = VOLTAGE_DEFAULT;
	float		CurrenThreshold  = CURRENT_DEFAULT;
	uint16_t	TimerThreshold	 = TIME_MAX_DEFAULT;
	
}SENSORS_THRESHOLD;



/************************************************************************/
/*			LCD_SHIELD BOTONS DEFINITION                                */
/************************************************************************/
/*
Valores de la entrada analÃ³gica para cada Boton externo.*/
/*
#define   Right   0
#define   Up      99
#define   Down    256
#define   Left    409
#define   Selec   640
#define   NoPuls  1023
*/

/*
*Como el shield del LCD incluye un conjunto de botones , todos ellos depende de una entrada analogica, 
ya que cada boton, representa una caida de tensiÃ³n en la entrada analogica. Por lo tanto he creado estos
define asociando cada rango al boton correspondiente.
*/

#define Right(In)     ((In>=0)   && (In<50))  ? 1:0
#define Up(In)        ((In>=50)  && (In<178)) ? 1:0
#define Down(In)      ((In>=178) && (In<332)) ? 1:0
#define Left(In)      ((In>=332) && (In<525)) ? 1:0
#define Selec(In)     ((In>=525) && (In<831)) ? 1:0
#define Empty(In)     (In>=831) ? 1:0


/************************************************************************/
/*			STRUCT DESIGN FOR MENUS                                     */
/************************************************************************/
typedef struct{
	
	uint8_t	VectorSize;
	uint8_t	MenuID;
	String	*MenuText;
}MENUS_LCD;

#endif /* INCFILE1_H_ */