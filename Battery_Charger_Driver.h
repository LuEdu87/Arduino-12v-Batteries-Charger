

#ifndef __BATTERY_CHARGER_DRIVER_H__
#define __BATTERY_CHARGER_DRIVER_H__

#include "LCD_Shield_Defines.h"



class Battery_Charger_Driver
{


private:

#ifdef SPANISH_LANGUAGE
	String	MainMenu[5]		= {"Iniciar Carga","Modi Corriente", "Modi Tiempo", "Modi Voltaje",""};
	String	ChargingMenu[4]	= {"V= ","I= ", "Time= ",""};
	String	FinalMenu[4]	= {"Paro X Corriente","Paro X Tiempo","Paro X Voltaje",""};
#endif

#ifdef ENGLISH_LANGUAGE
	String	MainMenu[5]		= {"Start","Modi Current", "Modi Time", "Modi Voltage",""};
	String	ChargingMenu[4]	= {"V= ","I= ", "Time= ",""};
	String	FinalMenu[4]	= {"Stop by Current","Stop by Time","Stop by Voltage",""};
#endif
	
		
	MENUS_LCD	Main={
		.VectorSize = (sizeof(MainMenu)/sizeof(MainMenu[0])),
		.MenuID	= 0,
		.MenuText = MainMenu
	};
	
	MENUS_LCD	Chargin={
		.VectorSize = (sizeof(ChargingMenu)/sizeof(ChargingMenu[0])),
		.MenuID	= 1,
		.MenuText = ChargingMenu
	};
	
	MENUS_LCD	Final={
		.VectorSize = (sizeof(FinalMenu)/sizeof(FinalMenu[0])),
		.MenuID	= 2,
		.MenuText = FinalMenu
	};
	
	
	/*Attributes to Manipulate the LCD Display.*/	
	MENUS_LCD	*Actual;		//Pointer to use in the LCD menus
	uint8_t		 Counter;		//Counter for the Menus Movement
	LiquidCrystal *LCD_SCREEN;	//Object for Manipulate the LiquidCrystal Library
	
	//Struct to Store de Data Sensors and Units Threshold(Voltage, Current and Time)
	SENSORS_THRESHOLD Measure;	//Struct for the Storage and Measure of Current, Voltage and Time
	
	//PinsNames for the Sensors and Rele. To Use with AnalogRead this are the default values
	uint8_t	BotonsPins	= BOTONS_PINS;			
	uint8_t CurrentPins	= CURRENT_SENSOR_PIN;
	uint8_t	VoltagePins	= VOLTAGE_SENSOR_PIN;
	uint8_t RelePin		= RELE_PIN;
	
	/*Flags for the Relee Shut*/
	uint8_t	FlagReleShut;
	uint8_t FlagEndCharg;
	uint8_t FlagStartDelay;
	
	/*Counter Cicles TImer2 Interruption*/
	uint16_t PreviousMillisCounter;
	uint16_t MillisCounter;
	
	/*Private Functions*/
	void ModifyValue(uint8_t Unit);
	float VoltageOneDecimal(void);
	float VoltageSameTime(void);
	void RelayPinActivation(uint8_t Flag);
	


public:
	Battery_Charger_Driver();
	Battery_Charger_Driver(uint8_t AnalogBotons,uint8_t AnalogCurrent,uint8_t AnalogVoltage, uint8_t PinRele);
	~Battery_Charger_Driver();
	
	
	void TimerOneCounter(void);
	uint8_t getMainID(void);
	void MenuMovement(uint16_t MeasureBoton);
	void MenuSelection(uint16_t MeasureBoton);
	void MenuDisplay(void);
	void ReadSensors(void);
	void ActivateRelay(void);
	


};

#endif //__BATTERY_CHARGER_DRIVER_H__
