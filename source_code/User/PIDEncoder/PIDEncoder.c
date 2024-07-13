#include "stm32f10x.h"
#include "PIDEncoder.h"

int Left_t,Right_t,Encoder_R,Encoder_L;
u8 key = 0;
u32 BMQMB = 7;
u32 BFBMQMB;

//增量式pid
typedef struct 
{
    //int      SetPoint;                               //设定目标 Desired Value
    double   Proportion;                               //比例常数 Proportional Const
    double   Integral;                                 //积分常数 Integral Const
    double   Derivative;                               //微分常数 Derivative Const
    int      LastError;                                //Error[-1]
    int      PrevError;                                //Error[-2]
}PID;

//定义PID相关宏
/*************************************/
#define  P_DATA      1.5                          //P参数
#define  I_DATA      0.1                          //I参数
#define  D_DATA      -0.4                         //D参数

static PID sPID;
static PID *sptr = &sPID;

/***PID参数初始化*******/
void PID_Init(void) 
{
    sptr->LastError=0;            //Error[-1]
    sptr->PrevError=0;            //Error[-2]
    sptr->Proportion=P_DATA;      //比例常数 Proportional Const
    sptr->Integral=I_DATA;        //积分常数  Integral Const
    sptr->Derivative=D_DATA;      //微分常数 Derivative Const
    //sptr->SetPoint=500;         //设定目标Desired Value
}

/********增量式PID控制设计******/
//NextPoint：当前输出值
//SetPoint： 目标值
int PID_Calc(int NextPoint,int SetPoint) 
{
		int iError,Outpid;                                   //当前误差
		iError=SetPoint-NextPoint;                           //增量计算
		Outpid=(sptr->Proportion * iError)                   //E[k]项
								-(sptr->Integral * sptr->LastError)      //E[k-1]项
								+(sptr->Derivative * sptr->PrevError);   //E[k-2]项
								
		sptr->PrevError=sptr->LastError;                     //存储误差，用于下次计算
		sptr->LastError=iError;
		return(Outpid);                                      //返回增量值
}

