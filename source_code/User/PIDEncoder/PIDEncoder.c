#include "stm32f10x.h"
#include "PIDEncoder.h"

int Left_t,Right_t,Encoder_R,Encoder_L;
u8 key = 0;
u32 BMQMB = 7;
u32 BFBMQMB;

//����ʽpid
typedef struct 
{
    //int      SetPoint;                               //�趨Ŀ�� Desired Value
    double   Proportion;                               //�������� Proportional Const
    double   Integral;                                 //���ֳ��� Integral Const
    double   Derivative;                               //΢�ֳ��� Derivative Const
    int      LastError;                                //Error[-1]
    int      PrevError;                                //Error[-2]
}PID;

//����PID��غ�
/*************************************/
#define  P_DATA      1.5                          //P����
#define  I_DATA      0.1                          //I����
#define  D_DATA      -0.4                         //D����

static PID sPID;
static PID *sptr = &sPID;

/***PID������ʼ��*******/
void PID_Init(void) 
{
    sptr->LastError=0;            //Error[-1]
    sptr->PrevError=0;            //Error[-2]
    sptr->Proportion=P_DATA;      //�������� Proportional Const
    sptr->Integral=I_DATA;        //���ֳ���  Integral Const
    sptr->Derivative=D_DATA;      //΢�ֳ��� Derivative Const
    //sptr->SetPoint=500;         //�趨Ŀ��Desired Value
}

/********����ʽPID�������******/
//NextPoint����ǰ���ֵ
//SetPoint�� Ŀ��ֵ
int PID_Calc(int NextPoint,int SetPoint) 
{
		int iError,Outpid;                                   //��ǰ���
		iError=SetPoint-NextPoint;                           //��������
		Outpid=(sptr->Proportion * iError)                   //E[k]��
								-(sptr->Integral * sptr->LastError)      //E[k-1]��
								+(sptr->Derivative * sptr->PrevError);   //E[k-2]��
								
		sptr->PrevError=sptr->LastError;                     //�洢�������´μ���
		sptr->LastError=iError;
		return(Outpid);                                      //��������ֵ
}

