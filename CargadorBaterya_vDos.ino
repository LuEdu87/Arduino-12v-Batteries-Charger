/*Begining of Auto generated code by Atmel studio */
#include "Battery_Charger_Driver.h"

/*End of auto generated code by Atmel studio */

Battery_Charger_Driver TestUno;
uint8_t FlagDisparo =0;


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t LecturaBoton=analogRead(A0);
  uint8_t  IDMenu= TestUno.getMainID();
  
  
  TestUno.MenuMovement(LecturaBoton); 
  TestUno.MenuSelection(LecturaBoton);
  /************************************************************************/
  /* Este Flag es para que el relee no arranque después del reseteo. Ya que una de las condiciones
     Es que solo cargue cuando le damos a "Iniciar Carga"                 */
  /************************************************************************/
  if (FlagDisparo==0)
  {
    TestUno.ActivateRelay();
    FlagDisparo=1;
  }
  
  /************************************************************************/
  /* Solo Lee los sensores cuando está en la pantalla de Iniciar carga
  /************************************************************************/
  if (IDMenu==1){
    
    TestUno.ReadSensors();
    TestUno.ActivateRelay();
    
  }
  
  
  TestUno.MenuDisplay();
  
  delay(100);
  
}
