// dlltest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include"dllfunction.h"
void line_fun(double a[NumOfParameter][NumOfParameter],double b[NumOfParameter],double p[NumOfParameter])//������˹��Ԫ������Է�����
{
	double x[NumOfParameter];//���Է�����Ľ� 
	double mm[NumOfParameter];//��������
	double T[NumOfParameter],BT=0;//�洢������ʱ����
	double temp=0;
	double t;//�������Ԫ
	int tap1=0,tap2=0;//�������Ԫ�±�
	int i,j,s;
	for(s=0;s<NumOfParameter;s++)
	{
		t=a[s][s];//����ֵ
		for(int p=s;p<NumOfParameter;p++)//ѡȡ�������Ԫ
		{
			if(fabs(a[p][s])>t)
			{
				t=a[p][s];
				tap1=p;
				tap2=s;
			}
		}
		if(t==0)
		{
			cout<<"ʧ��"<<endl;
			break;
		}
		if(tap1!=tap2)//����
		{
			for(int i=s;i<NumOfParameter;i++)
			{
				T[i]=a[tap1][i];
				a[tap1][i]=a[s][i];
				a[s][i]=T[i];
			}
			BT=b[tap1];//��B����
			b[tap1]=b[s];
			b[s]=BT;
		}
		for(i=s+1;i<NumOfParameter;i++)//��Ԫ
		{
			mm[i]=a[i][s]/a[s][s];
			for(j=s;j<NumOfParameter;j++)
			{
				a[i][j]=a[i][j]-mm[i]*a[s][j];
			}
			b[i]=b[i]-mm[i]*b[s];
		}
	}
	x[NumOfParameter-1]=b[NumOfParameter-1]/a[NumOfParameter-1][NumOfParameter-1];//�ش���
	for(i=(NumOfParameter-1)-1;i>=0;i--)
	{
		for(j=i+1;j<NumOfParameter;j++)
		{
			temp+=a[i][j]*x[j];
		}
		x[i]=(b[i]-temp)/a[i][i];
		temp=0;
	}
	for(i=0;i<NumOfParameter;i++)
	{
		p[i]=x[i];
	}
}

bool LestSqureFunctionSlution(double A[][NumOfParameter],double b[],int EqualtionNum,double x[NumOfParameter])
{	if(NumOfParameter>EqualtionNum)
		return false;
	int m=EqualtionNum;
	double ATA[NumOfParameter][NumOfParameter];
	double ATy[NumOfParameter];
	//���������С���˽�
	int i,j,k;
	for(i=0;i<NumOfParameter;i++)
	{
		ATy[i]=0;
		for(j=0;j<NumOfParameter;j++)
		{
			ATA[i][j]=0;
		}
	}


	for(i=0;i<NumOfParameter;i++)//����ATA[NumOfParameter][NumOfParameter]
	{
		for(j=0;j<NumOfParameter;j++)
		{
			for(k=0;k<m;k++)
			{
				ATA[i][j]+=A[k][i]*A[k][j];
			}
		}
	}


	for(i=0;i<NumOfParameter;i++)//����A[m][NumOfParameter]��b[m]������ATy[NumOfParameter]
	{
		for(j=0;j<m;j++)
		{
			ATy[i]+=A[j][i]*b[j];

		}
	}
	line_fun(ATA,ATy,x);//������Է�����
	return true;
}
//Xaxis��x������ Yaxis��Y��  EqualtionNum������   TuoYuan���ϵ��µ� 
bool EclipsParameterGet(int Xaxis[],int Yaxis[],int EqualtionNum,TuoYuan& OutValue)
{	
	using namespace std;
	double (*A)[NumOfParameter] = new double[EqualtionNum][NumOfParameter];
	double* b = new double[EqualtionNum];
	double Solution[NumOfParameter];
	for(int i = 0;i < EqualtionNum;i++)
		b[i] = -1;
	for(int i = 0;i < EqualtionNum;i++)
	{
		A[i][0] = Xaxis[i] * Xaxis[i];
		A[i][1] = Yaxis[i] * Yaxis[i];
		A[i][2] = Xaxis[i];
		A[i][3] = Yaxis[i];
	}
	if (LestSqureFunctionSlution(A,b,EqualtionNum,Solution))
	{
		delete[] A;
		delete[] b;
		double a = Solution[0];
		double b = 0;
		double c = Solution[1];
		double d = Solution[2];
		double e = Solution[3];

		double XC = (b * e - 2 * c * d) / (4 * a * c - b * b); 
		double YC = (b * d - 2 * a * e) / (4 * a * c - b * b);
		double longaix = 2*(a*XC*XC+c*YC*YC+b*YC*XC-1)/(a+c-sqrt((a-c)*(a-c)+b*b));
		longaix = sqrt(longaix);
		OutValue.top = XC - longaix;
		OutValue.bottom = XC + longaix;
		OutValue.centerX = XC;
		OutValue.centerY = YC;

		return true;
	}
	else
    {
		delete[] A;
	    delete[] b;
		return false;
	}
}
