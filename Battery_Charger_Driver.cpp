

#include "Battery_Charger_Driver.h"


// CONSTRUCTOR
Battery_Charger_Driver::Battery_Charger_Driver()
{
		Actual=&Main;
		Counter=0;
		FlagReleShut=0;
		FlagStartDelay=0;
		pinMode(2,OUTPUT);
		pinMode(3,OUTPUT);
		LCD_SCREEN = new LiquidCrystal(8, 9, 4, 5, 6, 7);
		LCD_SCREEN->begin(16,2);		
		
} 

//CONSTRUCTOR 2
Battery_Charger_Driver::Battery_Charger_Driver(uint8_t AnalogBotons,uint8_t AnalogCurrent,uint8_t AnalogVoltage, uint8_t PinRele){
	
	Actual=&Main;					//Initialize the Struct Pointer to the Main Menu
	Counter=0;						//Counter for the menu movement
	FlagReleShut=0;					//Flag for the Relay Shut
	FlagStartDelay=0;				//Flag for the Initial Peak Current
	BotonsPins	= AnalogBotons;		//Analog Pin for LCD Keypad Shield Buttons
	CurrentPins	= AnalogCurrent;	//Analog Pin for Current Sensor
	VoltagePins	= AnalogVoltage;	//Analog Pin for Voltage Sensor
	RelePin		= PinRele;
	//LCD Keypad Shield Pins		RS	En	DB4	DB5 DB6 DB7
	LCD_SCREEN	= new LiquidCrystal( 8,  9,  4,  5,  6,  7);
	LCD_SCREEN->begin(16,2);
		
}

// DESTRUCTOR
Battery_Charger_Driver::~Battery_Charger_Driver()
{
	delete LCD_SCREEN;
} 






/************************************************************************/
/* Function to return the MenuID of the actual Menu                     */
/************************************************************************/
uint8_t Battery_Charger_Driver::getMainID(void){
	
	return Actual->MenuID;
}


/************************************************************************/
/*            Function for the movement in the display Menu             */
/************************************************************************/
void Battery_Charger_Driver::MenuMovement(uint16_t MeasureBoton){
	
	if(Down(MeasureBoton)){
		
		Counter++;
		
	}else if(Up(MeasureBoton)){
		
		Counter--;
		
	}
	
	
	if ((Counter>Actual->VectorSize)&&(Counter<=255)){
		
		Counter=0;
		
	}else if((Counter>(Actual->VectorSize-2))){
		
		Counter=(Actual->VectorSize-2);
		
	}
}




/************************************************************************/
/*  Method for the movement inside of MenuLCD, using an Analogic Input  */
/*of the shield                                                         */                                                                     
/************************************************************************/
void Battery_Charger_Driver::MenuSelection(uint16_t MeasureBoton){
	/*Este if y switch es para la entrada en los submenus en función del menú en el que estén. Para localizar nos basamos en el partado Posicion que tiene cada STRUCT al crease, que debe ser
  único para cada uno. Así sabemos en cual estamos y podemos asignarles al puntero struct al siguiente menú donde queramos ir*/
  
  if(Selec(MeasureBoton)){

  /*Switch->Analizo la variable contador, que su valor varía en función de si pulso hacía arriba o hacia abajo. Por lo tanto sabiendo en que menú estoy (Struct.posicion) y el valor
  del contador puedo ir hacia un submenú*/
  
    switch(Counter){ 
                          
      case(0):
        /*Este es la pantalla principal->"Iniciar Carga" "Por Corriente" "Por Tiempo" "Por Voltaje" */
        if(Actual->MenuID==0){           //Posicion 1 es Principal y si mi contador está en 0 vamos al submenú corriente.
          Actual = &Chargin;
          Counter=0;
		}
		
      break;


      case(1):
                
        if(Actual->MenuID==0){
          
          ModifyValue(0);//
          Counter=0;
        }
        
        break;


      case(2):
                
        if(Actual->MenuID==0){
          
          ModifyValue(1); 
		  Counter=0;
        }
		
      break;


      case(3):
                
        if(Actual->MenuID==0){
        
          ModifyValue(2);
          
        }
      
	    Counter=0;
        break;
      
    }
  }
  
  else if(Left(MeasureBoton)){
    
		Actual=&Main;  
        Counter=0;
     
  }
}




/************************************************************************/
/* Function for Display Menus in the LCD through LCD library            */
/************************************************************************/


void Battery_Charger_Driver::MenuDisplay(){
	
	 
  /*   *
        *
  ********
  ********
        *
       *
  */
  uint8_t flecha[8]={
    0b00000000,
    0b00000100,
    0b00000010,
    0b11111111,
    0b11111111,
    0b00000010,
    0b00000100,
    0b00000000,    
  };
  
 
  
  if(Actual->MenuID==1){
     LCD_SCREEN->clear();
     LCD_SCREEN->setCursor(1,0);
     LCD_SCREEN->print(Actual->MenuText[0]);
     LCD_SCREEN->setCursor(4,0);
     LCD_SCREEN->print(Measure.VoltageSensor);
     LCD_SCREEN->setCursor(9,0);
     LCD_SCREEN->print(Actual->MenuText[1]);
     LCD_SCREEN->setCursor(12,0);
     LCD_SCREEN->print(Measure.CurrentSensor);
     LCD_SCREEN->setCursor(1,1);
     LCD_SCREEN->print(Actual->MenuText[2]);
     LCD_SCREEN->setCursor(7,1);
     LCD_SCREEN->print(Measure.TimerCounter);
	 
  }else if (Actual->MenuID==2){
	  
	  LCD_SCREEN->clear();
	  LCD_SCREEN->setCursor(1,0);
	  LCD_SCREEN->print(Actual->MenuText[FlagEndCharg]);
	  LCD_SCREEN->setCursor(1,1);
	  LCD_SCREEN->print("Time: ");
	  LCD_SCREEN->setCursor(7,1);
	  LCD_SCREEN->print(Measure.TimerCounter);
	  
  }
  else{  
     LCD_SCREEN->createChar(0,flecha);
     LCD_SCREEN->clear();
     LCD_SCREEN->setCursor(0,0);
     LCD_SCREEN->write(uint8_t(0));
     LCD_SCREEN->setCursor(1,0);
     LCD_SCREEN->print((Actual->MenuText[Counter]));
     LCD_SCREEN->setCursor(1,1);
     LCD_SCREEN->print((Actual->MenuText[Counter+1]));
  }
}



/************************************************************************/
/* Function to Change Pred Values of Current, Voltage and Time          */
/************************************************************************/

void Battery_Charger_Driver::ModifyValue(uint8_t Unit){
	/*Unit =0 -> Corriente/Current*/
	/*Unit =1 -> Tiempo/Time*/
	/*Unit =2 -> Voltaje/Voltage*/
	delay(400);
	
	uint16_t LecturaBoton;
	uint8_t Posicion=0;
	uint16_t PtrValor[3];
	uint8_t CuadradoPosi[3];

	//Unidad para Corriente
	if(Unit==0){
		PtrValor[0]=0;
		PtrValor[1]=0;
		PtrValor[2]=3;
		CuadradoPosi[0]=14;
		CuadradoPosi[1]=13;
		CuadradoPosi[2]=11;
	}
	//Unidad para tiempo
	else if(Unit==1){
		PtrValor[0]=0;
		PtrValor[1]=0;
		PtrValor[2]=0;
		CuadradoPosi[0]=13;
		CuadradoPosi[1]=12;
		CuadradoPosi[2]=11;
	}
	//Unidad Voltaje
	else if(Unit==2){
		PtrValor[0]=0;
		PtrValor[1]=5;
		PtrValor[2]=1;
		CuadradoPosi[0]=14;
		CuadradoPosi[1]=12;
		CuadradoPosi[2]=11;
	}
#ifdef SPANISH_LANGUAGE
	String Units[3]={"Amperios: ","Tiempo: ","Voltios: "};
#endif

#ifdef ENGLISH_LANGUAGE
	String Units[3]={"Ampers: ","Time: ","Volts: "};
#endif
	

	byte Cuadrado[8]={
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	};
	
	
	LCD_SCREEN->createChar(1,Cuadrado);
	LecturaBoton=analogRead(BotonsPins);

	
	while(!Selec(LecturaBoton)){

		LecturaBoton=analogRead(BotonsPins);
		if(Right(LecturaBoton)){
			if((Posicion<=255)&&(Posicion>1)){
				Posicion=2;
			}
			else 
				Posicion--;
			
		}

		if(Left(LecturaBoton)){
			if((Posicion<=255)&&(Posicion>1)){
				Posicion=0;
			}
			else 
				Posicion++;
		}
		
		if(Down(LecturaBoton)){
			if(PtrValor[Posicion]<1){
				PtrValor[Posicion]=9;
			}
			else
			PtrValor[Posicion]--;
			
		}

		else if(Up(LecturaBoton)){
			if(PtrValor[Posicion]>8){
				PtrValor[Posicion]=0;
			}
			else
			PtrValor[Posicion]++;
			
		}


		//Valor cuadrado de modificación
		if(Posicion==0){
			LCD_SCREEN->setCursor(CuadradoPosi[0],0);
			LCD_SCREEN->write(byte(1));
		}
		
		else if(Posicion==1){
			LCD_SCREEN->setCursor(CuadradoPosi[1],0);
			LCD_SCREEN->write(byte(1));
		}

		else if(Posicion==2){
			LCD_SCREEN->setCursor(CuadradoPosi[2],0);
			LCD_SCREEN->write(byte(1));
		}
		delay(250);
		LCD_SCREEN->clear();
		
		
		
		LCD_SCREEN->setCursor(1,0);
		LCD_SCREEN->print(Units[Unit]);
		
		if(Unit==0){								//			File to Modify the Time in the "Modi Corriente"
			LCD_SCREEN->setCursor(11,0);
			LCD_SCREEN->print(PtrValor[2]);
			LCD_SCREEN->setCursor(12,0);
			LCD_SCREEN->print(".");
			LCD_SCREEN->setCursor(13,0);
			LCD_SCREEN->print(PtrValor[1]);
			LCD_SCREEN->setCursor(14,0);
			LCD_SCREEN->print(PtrValor[0]);
			
		}else if(Unit==1){							//			File to Modify the Time in the "Modi Tiempo"
			
			LCD_SCREEN->setCursor(11,0);
			LCD_SCREEN->print(PtrValor[2]);
			LCD_SCREEN->setCursor(12,0);
			LCD_SCREEN->print(PtrValor[1]);
			LCD_SCREEN->setCursor(13,0);
			LCD_SCREEN->print(PtrValor[0]);
			
		}else if(Unit==2){							//			File to Modify the Time in the "Modi Tiempo"
			LCD_SCREEN->setCursor(11,0);
			LCD_SCREEN->print(PtrValor[2]);
			LCD_SCREEN->setCursor(12,0);
			LCD_SCREEN->print(PtrValor[1]);
			LCD_SCREEN->setCursor(13,0);
			LCD_SCREEN->print(".");
			LCD_SCREEN->setCursor(14,0);
			LCD_SCREEN->print(PtrValor[0]);
		}
		
		
		//Code for the second file , where appears the last value of Current or Voltage or Time Limit.
		LCD_SCREEN->setCursor(1,1);
		LCD_SCREEN->print("ValActual: ");
		LCD_SCREEN->setCursor(11,1);
		
		if (Unit==0){
			LCD_SCREEN->print(Measure.CurrenThreshold);
			delay(250);
		}else if(Unit==1){
			LCD_SCREEN->print(Measure.TimerThreshold);
			delay(250);
		}else if(Unit==2){
			LCD_SCREEN->print(Measure.VoltageThreshold);
			delay(250);
		}
		
		
		
		
	}
	
	/*Saving the Modified Data into the Threshold Variables*/
	if(Unit==0){//Current
		
		Measure.CurrenThreshold=PtrValor[2]+PtrValor[1]*(0.1)+PtrValor[0]*(0.01);
		
	}else if(Unit==1){//Time
		
		Measure.TimerThreshold=PtrValor[2]*100+PtrValor[1]*10+PtrValor[0];
		
	}else if(Unit==2){//Voltage
		
		Measure.VoltageThreshold=PtrValor[2]*10+PtrValor[1]+PtrValor[0]*(0.1);
		
	}
}



void Battery_Charger_Driver::ReadSensors(){
	TimerOneCounter();
	float TempCurrent=0,TempVoltage=0,HallVoltaje=0;

	for(int i=0;i<=MEASURE_SAMPLES;i++){

		HallVoltaje	=	analogRead(CurrentPins)*(5.0/1023.0);
		TempCurrent	+=  (HallVoltaje-2.5)/CURRENT_SENSIBILITY;
		TempVoltage	+=  map(analogRead(VoltagePins),   0.0,  1023.0, 0.0,  25.0);
	}

	Measure.CurrentSensor	 = TempCurrent/MEASURE_SAMPLES;
	Measure.VoltageSensor    = TempVoltage/MEASURE_SAMPLES;	
	
}



/************************************************************************/
/* Function to Convert the measure of the Sensor in Only one Decimal    */
/************************************************************************/
float Battery_Charger_Driver::VoltageOneDecimal(){
	
	float OneDecimalVoltage;
	
	
	/*Var2=(int)Var+(int)(((Var-(int)Var)*10))*0.1; Formula para implementar para comprobar que el valor no haya cambiado en X tiempo.
	*Esta fórmula es para guardar en la variable el valor del sensor pero solo con un decimal. Por ejuemplo 14.254 lo que hace la formula es
	*14.2. De esta manera es más fácil hacer la comparación, ya que no tiene tantos valores que cambian, lo cual perdujicaria a la comparación
	*del condicional IF de abajo
	*/
	
	OneDecimalVoltage=(int)Measure.VoltageSensor+(int)(((Measure.VoltageSensor-(int)Measure.VoltageSensor)*10))*0.1;
	
	return OneDecimalVoltage;
	


}



/************************************************************************/
/* Function to Counter the time that the voltage doesn´t change         */
/************************************************************************/
float Battery_Charger_Driver::VoltageSameTime(){
	
	float static PreviousVoltage;
	float static SameVoltageTime;
	float static PreviousTimer;
	
	float TempVoltageOneDecimal;
	TempVoltageOneDecimal=VoltageOneDecimal();
	
	/*
	*Este if es para controlar el tiempo que está el voltaje sin cambiar. Por cada vez que el voltaje se mantenga, creo un temporizador interno, usando el tiempo de la variable tiempo
	*del struct. Si son iguales, entonces hacemos la resta entre el último valor guardado del temporizador y el valor actual, dando la direferencia de tiempo transcurrido, y lo guardamos
	*en la variable TiempoMismoVoltaje.
	*Si por el cntrario los valores no son iguales, igualo la variable TiempoAnterior al tiempo actual.(dando una diferencia de 0) y resetamos la variable que guarda la diferencia entre
	*el ultimo tiempo y el tiempo actual. Dicha variable si supera un valor predeterminado MINS_COMPARAC( Es un define en SensoresyRele.h)
	*/
	if((TempVoltageOneDecimal==PreviousVoltage)){
		SameVoltageTime=Measure.TimerCounter-PreviousTimer;    //Si el valor del sensor no ha cambiado, entonces restamos la diferencia entre ambos y lo guardamos
	}
  
	else if((TempVoltageOneDecimal!=PreviousVoltage)){
		PreviousTimer=Measure.TimerCounter;
		SameVoltageTime=0;
	}
	
	PreviousVoltage=TempVoltageOneDecimal;
	
	return SameVoltageTime;
	
}



/************************************************************************/
/* Function for the Manipulation of Relay with the sensors Measure      */
/************************************************************************/
void Battery_Charger_Driver::ActivateRelay(){
	
	uint8_t static PreviousFlagReleShut=0;
	float	TempSameVoltage;
	
	TempSameVoltage=VoltageSameTime();
  
	//Initial Delay for the Initial Current's Peak of 1 Second.
	if(FlagStartDelay==0){
		delay(1000);
		FlagStartDelay=1;		
	}
	
	//Con este IF comparamos los sensores de corriente y voltaje con los umbrales que hemso programado por pantalla, si los supera entonces activamso el flag.
	if((Measure.CurrentSensor>=Measure.CurrenThreshold)||(Measure.VoltageSensor>=Measure.VoltageThreshold)||(Measure.TimerCounter>=Measure.TimerThreshold)||(TempSameVoltage>=TIME_MAX_COMPARING)||(Actual->MenuID!=1)){
		
		FlagReleShut=1;
		if (Measure.CurrentSensor>=Measure.CurrenThreshold){
			
			FlagEndCharg = 0;
			Actual=&Final;
			
		}else if((Measure.TimerCounter>=Measure.TimerThreshold)||(TempSameVoltage>=TIME_MAX_COMPARING)){
			
			FlagEndCharg = 1;
			Actual=&Final;
			
		}else if (Measure.VoltageSensor>=Measure.VoltageThreshold){
			
			FlagEndCharg = 2;
			Actual=&Final;
		}
	
    
	}
  
	else if((Measure.CurrentSensor < Measure.CurrenThreshold)&&(Measure.VoltageSensor < Measure.VoltageThreshold)&&(Measure.TimerCounter < Measure.TimerThreshold)&&(TempSameVoltage < TIME_MAX_COMPARING)&&(Actual->MenuID==1)){//&&(FlagReleShut==0)
		FlagReleShut=0;
    
	}
  
	/*
	*Esta función se encarga de enviar los parametros a los reles de disparo. con ella podemos controlar el relee en función de los flags de disparo. En futuras versiones hay que revisar
	*como hacerlo con una puerta XOR con los distintos flags para así solo escribir solo cuando detecte un cambio.
	*if(FlagDisparo^FlagAnterior){}, con esto podemos detectar el cambio de valor de la variable. USando la puerta XOR, 
	*NOTA: se prueba con la funciona normal, pero cuando implementamos los menús produce un error en el rearranque y para que funcione, hay que bajar la tensión de la batería por debajo del 
	*umbral de disparo y se vuelve a reactivar los flags. Posiblemente sea un problema de lógica con la puerta XOR y los flancos de disparo.
	*
	*Codigo Original:
	*/
	if(FlagReleShut^PreviousFlagReleShut){
	
		if(FlagReleShut==0){	 
		
			RelayPinActivation(FlagReleShut);
   
		}else if(FlagReleShut==1){
		
			RelayPinActivation(FlagReleShut);
		}
	}
	
 

	//Antes  de abandonar la función, actualizamos los dos valores.Que al ser Static ambas, su valor no se borra una vez se sale de la función.
	PreviousFlagReleShut = FlagReleShut;

}


/************************************************************************/
/*                           Function to Activate the Relay Pins        */
/************************************************************************/

void Battery_Charger_Driver::RelayPinActivation(uint8_t Flag){
	
	if(Flag==1){
		
		digitalWrite(RelePin,HIGH);
		digitalWrite(3,HIGH);
		
	}else if(Flag==0){
		
		digitalWrite(RelePin,LOW);
		digitalWrite(3,LOW);
	}
}



/************************************************************************/
/* Function for the Timer                                               */
/************************************************************************/


void Battery_Charger_Driver::TimerOneCounter(void){
	
	MillisCounter=millis()-PreviousMillisCounter;
	if (MillisCounter>=60000){
		
		Measure.TimerCounter++;
		PreviousMillisCounter=millis();
		MillisCounter=0;
		
	}
	
	
	
}








