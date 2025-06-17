#include <stdio.h>
#include "../inc/kf_altitude.h"


int main()
{
   unsigned short int i = 0; 

   printf( "Simple Altitude Kalman Filter.\r\n" );
   kf_setup_initial_value();

   do
   {
      float altitude, acceleration;
      i++;
      printf( "Iteration %u.\r\n", i );
      
      printf( "Insert altitude:" );
      scanf( "%f", &altitude );   
      
      printf( "Insert acceleration:" );
      scanf( "%f", &acceleration );  
      
      kf_update( altitude, acceleration );
      
      printf( "\nKalman computed altitude: %f\r\n", get_filtered_altitude() );
   
   }while(1);


   return 0;
}
