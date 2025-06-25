#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "../inc/kf_altitude.h"


int main()
{
   FILE* fptr_i;
   FILE* fptr_o;
   char line[50];
   char buf[10];
   float altitude = 0, acceleration = 0;
   

   printf( "Simple Altitude Kalman Filter.\r\n" );
   kf_setup_initial_value();

   fptr_i = fopen("./test/example.csv", "r"); //Input file 
   fptr_o = fopen("./test/filtered.csv", "w"); //Output file
   if( fptr_i == NULL )
   {
   
   	printf( "File opening failed.\r\n" );
   	return -1;
   	
   }
   fgets( line, sizeof(line), fptr_i ); //Avoid the first line


   while( fgets( line, sizeof(line), fptr_i ) )
   {
   	char* token;
   	
   	token = strtok(line, ",");
        if (token != NULL) 
        {
        
            altitude = atof(token);
            
        } 
        else 
        {
        
            continue;
            
        }


        token = strtok(NULL, "\n"); 
        if (token != NULL) 
        {
        
            acceleration = atof(token);
        
        } 
        else 
        {
        
            continue;
            
        }
        
        
      	kf_update( altitude, acceleration );

        printf("Altitude: %.4fm, Filtered: %.4fm\n", altitude, get_filtered_altitude());
   	
   	gcvt( get_filtered_altitude(), 8, buf );
   	fputs( buf, fptr_o );
   	fputs( ",", fptr_o );
   	
   	gcvt( get_filtered_velocity(), 8, buf );
   	fputs( buf, fptr_o );
   	fputs( ",", fptr_o );
   	
   	gcvt( get_filtered_acceleration(), 8, buf );
   	fputs( buf, fptr_o );
        fputs( "\n", fptr_o );
  
   }

   fclose( fptr_i );
   fclose( fptr_o );

   return 0;
}

/*
Old code

      // Opening the file in read mode
      fptr = fopen("filename.txt", "r");
      float altitude, acceleration;
      i++;
      printf( "Iteration %u.\r\n", i );
      
      printf( "Insert altitude:" );
      scanf( "%f", &altitude );   
      
      printf( "Insert acceleration:" );
      scanf( "%f", &acceleration );  
      
      kf_update( altitude, acceleration );
      
      printf( "\nKalman computed altitude: %f\r\n", get_filtered_altitude() );
*/

