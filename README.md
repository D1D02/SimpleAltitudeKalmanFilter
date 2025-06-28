# Simple Altitude Kalman Filter
This is a simple example of a Kalman Filter used for altitude estimation. This project is born to filter data from two sensors: an altitude sensor and an acceleration sensor. In the example, 
I have tuned the Kalman Filter specifically for the lps22hb barometric sensor and lm6dso acceleration sensor ( both producted by STMicroelectronics, maybe I will release an STM32 example
of a very simple flight computer ). **Remember**: you can disassemble this code and insert it in your project, and also to make modifications ( if you improve something, it would be nice
if you propose modifications as an issue ).

> [!NOTE]
> This is a student project and is maintained only by the creator himself.

> [!WARNING]
> This library could have problems that are not resolved: in that case, open an issue if it's not already opened.

> [!CAUTION]
> The library creator is not responsible for any direct, indirect, incidental, consequential, or special damages arising out of or in any way connected with the use or misuse of this code.
> Users are solely responsible for their actions and any outcomes resulting from the use of this code. By using this code, you agree to assume all risks and liabilities associated with its use.


## Installation Guide
The library is based on another library for the matrix definition and calculations ( a library also made by me ), so there are some steps to do before you can use it. First step is to 
install the matrix library ( after you have cloned this library ):
```
mkdir lib
cd lib/
git clone git@github.com:D1D02/c-matrix-float.git
```

> [!WARNING]
> This code is going to work on Linux but not on other platforms ( for now ).

After you have done this, you can use the library: to run the example code, you just need to execute this command that will compile all the necessary and will open the GUI for plotting 
data.
```
python plot.py
```
After you prompt this command, the expected output is something like this:
![Plot_Example](https://github.com/user-attachments/assets/10994019-a898-4efa-a1f4-2702f3a48e31)
This is all to test the library: if you find any problem, create an issue explaining it.

## Test
You can test using your data changing the data set from the folder test:
```
/test/example.csv
```
To correctly filter and display data, you need to keep the right format in the .csv file. Just follow this path:
```
Alt(A),Acc(z)
28,0
29.820404,56.80202
35.398577,46.880117
43.552225,42.163497
```
on the Alt(A) column insert Altitude, on the Acc(z) column insert the Acceleration on the z-axis.

## Kalman Filter Model
## Prediction

In the prediction phase, the filter estimates the current state and its uncertainty based on the previous state estimate and the system's dynamics.

$$
\begin{align*}
\mathbf{x}_k &= \mathbf{A} \cdot \mathbf{x}_k \\
\mathbf{P}_k &= \mathbf{A} \cdot \mathbf{P}_k \cdot \mathbf{A}^\text{T} + \mathbf{Q} \\
\mathbf{y} &= \mathbf{z}_k - \mathbf{H} \cdot \mathbf{x}_k \quad \\
\mathbf{K} &= \mathbf{P}_k \cdot \mathbf{H}^\text{T} \cdot (\mathbf{H} \cdot \mathbf{P}_k \cdot \mathbf{H}^\text{T} + \mathbf{R})^{-1} \quad \\
\mathbf{x}_k &= \mathbf{x}_k + \mathbf{K} \cdot \mathbf{y} \quad \\
\mathbf{P}_k &= (\mathbf{I} - \mathbf{K} \cdot \mathbf{H}) \cdot \mathbf{P}_k
\end{align*}
$$

> [!WARNING]
> There are some problems about subscript not working properly: I will fix it asap.

## Filter tuning
If you want to change the kalman filter parameters, you can modify .c files. To alter the delta sampling time, you need to navigate to **/inc/kf_altitude.h** and modify 
this row:
```
#define DELTA_T 0.250  //Average delta time between every Kalman Filter update
```
Time is expressed in seconds: in this case, sampling is every 1/4 of second.

For other parameters, you need to navigate to **/inc/kf_altitude_constants.h**.

***Initial Uncertainty***
```
#define DELTA_INITIAL_ALTITUDE 1.0f
#define DELTA_INITIAL_VELOCITY 10.0f
#define DELTA_INITIAL_ACCELERATION 100.0f
```

***Initial Process Noise***
```
#define NOISE_ALTITUDE 0.01f
#define NOISE_VELOCITY 0.02f
#define NOISE_ACCELERATION 0.001f
```

***Initial Sensor Noise***
```
#define RMS_ALTITUDE 0.06f
#define RMS_ACCELERATION 0.003f
```
