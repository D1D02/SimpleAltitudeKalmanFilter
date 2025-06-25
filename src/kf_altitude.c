#include "../inc/kf_altitude.h"

//State Vector
matrix_float * x_pre;
matrix_float * x_k_n;

//Transition Matrix
matrix_float * A;

//Control Matrix
//Not present

//Covariance Matrix
matrix_float * P_pre;
matrix_float * P_k_n;

//Process Noise
matrix_float * Q_k;

//Observation Matrix
matrix_float * H_k;

//Measure Noise Matrix
matrix_float * R_noise;

//Kalman Gain
matrix_float * K_k;

//Measured and Expected Observation
matrix_float * z_k;
matrix_float * y_k;

//Identity Matrix
matrix_float * I;


void kf_setup_initial_value()
{
   //State Vector Initialization
   x_pre = create_empty_float_matrix( 3, 1 );
   
   //Transition Matrix Initialization
   A = transition_matrix_init();
	
   //Covariance Matrix Initialization
   P_pre = covariance_matrix_init();
   P_k_n = create_empty_float_matrix( 3, 3 );
   
   //Process Noise Initialization
   Q_k = process_noise_matrix_init();
   
   //Observation Matrix Initialization
   H_k = observation_matrix_init();
   
   //Measure Noise Matrix Initialization
   R_noise = measure_noise_matrix_init();
   
   //Kalman Gain Initialization
   K_k = create_empty_float_matrix( 3, 2 );
        
   //Measured and Expected Observation Matrix Initialization
   z_k = create_empty_float_matrix( 2, 1 );
   y_k = create_empty_float_matrix( 2, 1 );
   
   //Identity Matrix Initialization
   I = create_identity_matrix(3);
   
}

void kf_update( const float altitude, const float acceleration )
{
   
   z_k->p_matrix[0][0] = ( float ) altitude;
   z_k->p_matrix[1][0] =  ( float )  acceleration;
   
   // 1. State Vector
   free_matrix_float( x_k_n );
   x_k_n = product_matrices( A, x_pre );
   
   // 2. Uncertainty 
   free_matrix_float( P_k_n );
   P_k_n = sum_matrices( product_matrices( product_matrices( A, P_pre), transpose_matrix( A ) ), Q_k );

   // 3. Expected Observation
   free_matrix_float( y_k );
   y_k = difference_matrices(z_k, product_matrices( H_k, x_k_n ) );

   // 4. Kalman Gain
   free_matrix_float( K_k );
   matrix_float * tmp = sum_matrices( product_matrices( H_k, product_matrices( P_k_n, transpose_matrix( H_k ) ) ), R_noise);
   K_k = product_matrices( product_matrices ( P_k_n,  transpose_matrix( H_k ) ) , inverse_matrix( tmp ) );

   // 5. Update State Estimation
   free_matrix_float( x_pre );
   x_pre = sum_matrices( product_matrices( K_k, y_k ), x_k_n ); 
   
   // 6. Covariance Correction
   free_matrix_float( P_pre );
   P_pre = product_matrices( difference_matrices( I, product_matrices( K_k, H_k ) ), P_k_n);
   
}

float get_filtered_altitude()
{
   return x_pre->p_matrix[0][0];
}

float get_filtered_velocity()
{
   return x_pre->p_matrix[1][0];
}

float get_filtered_acceleration()
{
   return x_pre->p_matrix[2][0];
}

matrix_float * transition_matrix_init()
{
   matrix_float * matrix = create_empty_float_matrix( 3, 3 );
   if( matrix == NULL )
      return NULL;
      
   //First row
   matrix->p_matrix[0][0] = 1;
   matrix->p_matrix[0][1] = DELTA_T;
   matrix->p_matrix[0][2] = ( DELTA_T * DELTA_T ) / 2;
   
   //Second row
   matrix->p_matrix[1][1] = 1;
   matrix->p_matrix[1][2] = DELTA_T;

   //Third row
   matrix->p_matrix[2][2] = 1;
   
   return matrix;
}

matrix_float * covariance_matrix_init()
{
   matrix_float * matrix = create_empty_float_matrix( 3, 3 );
   if( matrix == NULL )
      return NULL;
      
   //First row
   matrix->p_matrix[0][0] = RHO_SQUARED_INITIAL_ALTITUDE_UNCERTAINTY;
   
   //Second row
   matrix->p_matrix[1][1] = RHO_SQUARED_INITIAL_VELOCITY_UNCERTAINTY;

   //Third row
   matrix->p_matrix[2][2] = RHO_SQUARED_INITIAL_ACCELERATION_UNCERTAINTY;
   
   return matrix;
}

matrix_float * process_noise_matrix_init()
{
   matrix_float * matrix = create_empty_float_matrix( 3, 3 );
   if( matrix == NULL )
      return NULL;
      
   //First row
   matrix->p_matrix[0][0] = RHO_SQUARED_INITIAL_ALTITUDE_NOISE;
   
   //Second row
   matrix->p_matrix[1][1] = RHO_SQUARED_INITIAL_VELOCITY_NOISE;

   //Third row
   matrix->p_matrix[2][2] = RHO_SQUARED_INITIAL_ACCELERATION_NOISE;
   
   return matrix;
}

matrix_float * observation_matrix_init()
{
   matrix_float * matrix = create_empty_float_matrix( 2, 3 );
   if( matrix == NULL )
      return NULL;
      
   //First row
   matrix->p_matrix[0][0] = 1;
   
   //Second row
   matrix->p_matrix[1][2] = 1;
   
   return matrix;
}

matrix_float * measure_noise_matrix_init()
{
   matrix_float * matrix = create_empty_float_matrix( 2, 2 );
   if( matrix == NULL )
      return NULL;
      
   //First row
   matrix->p_matrix[0][0] = RHO_SQUARED_ALTITUDE_RMS;
   
   //Second row
   matrix->p_matrix[1][1] = RHO_SQUARED_ACCELERATION_RMS;
   
   return matrix;
}

