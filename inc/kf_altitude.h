#ifndef KF_ALTITUDE_H
#define KF_ALTITUDE_H

#include "kf_altitude_constants.h"
#include "../lib/c-matrix-float/matrix.h"

#define DELTA_T 0.250  //Average delta time between every Kalman Filter update

void kf_setup_initial_value();
void kf_update( const float , const float );
float get_filtered_altitude();

//Initialization functions
matrix_float * transition_matrix_init();
matrix_float * covariance_matrix_init();
matrix_float * process_noise_matrix_init();
matrix_float * observation_matrix_init();
matrix_float * measure_noise_matrix_init();

#endif
