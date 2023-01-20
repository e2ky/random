# include "glcd_test.h"

char trial=5;

void setup() 
{
  GLCD_Init();    /* Initialize GLCD */
}

void loop() 
{
  GLCD_ClearAll(); /* Clear all GLCD display */
  GLCD_String(0, trial); /* Print String on 0th page of display */
  GLCD_String(7,"okay");
  delay (1000);
  GLCD_ClearAll(); /* Clear all GLCD display */
  GLCD_String(0,"Atmel" "     verify"); /* Print String on 0th page of display */
  GLCD_String(7,"okay2");
  delay (1000);
}
