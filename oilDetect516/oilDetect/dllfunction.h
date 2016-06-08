#pragma once
#include<iomanip>
#include<math.h>
#include <iostream>
#define NumOfParameter 4
typedef struct  
{   
	int top;
	int bottom;
	int centerX;
	int centerY;
}TuoYuan;
using namespace std;
void line_fun(double a[NumOfParameter][NumOfParameter],double b[NumOfParameter],double p[NumOfParameter]);
bool LestSqureFunctionSlution(double A[][NumOfParameter],double b[],int EqualtionNum,double x[NumOfParameter]);
bool EclipsParameterGet(int Xaxis[],int Yaxis[],int EqualtionNum,TuoYuan& OutValue);