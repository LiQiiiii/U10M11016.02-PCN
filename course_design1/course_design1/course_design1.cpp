//题目:模拟总线型以太网数据帧发送过程

#include "stdafx.h"
#include "course_design1.h"
#include "cmath"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//总线上连接的计算机个数为2个，支持CSMA\CD协议
CWinThread *thread1,*thread2;
//用两个线程a和b模拟以太网上的两台主机。用一个双字类型变量Bus模拟总线。
DWORD ID1,ID2,Bus=0;
UINT aThread(LPVOID pParam);
UINT bThread(LPVOID pParam);

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化MFC
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		thread1=AfxBeginThread(aThread,NULL);//启动线程a
		ID1=thread1->m_nThreadID;
		thread2=AfxBeginThread(bThread,NULL);//启动线程b
		ID2=thread2->m_nThreadID;
		getchar();//主线程等待，按任意键退出程序
	}

	return nRetCode;
}

UINT aThread(LPVOID pParam)
{
	int i=0;//发送成功次数
	int CollisionCounter=16;//冲突计数器初始值为16
	//随机延迟算法中的冲突窗口值取0.005
	double collisionWindow=0.005;
	int randNum=rand()%3;
Loop:if(Bus==0)//总线空闲
	 {
		 Bus=Bus|ID1;//模拟发送包
		 Sleep(2);
		 if(Bus==ID1)//若判断为无冲突
		 {
			 //数据发送成功（Bus==ID），报告"xxx send success"
			 printf("%d send success\n",ID1);
			 Bus=0;//内存清零
			 CollisionCounter=16;//复原冲突计数器
			 Sleep(rand()%10);//经过一个随即延时
			 i++;
			 //在主机发送成功次数增加后，报告已发送成功的次数
			 printf("主机a成功发送%d次\n",i);
			 if(i<5)//每台主机须向总线上成功发送5次数据
				 goto Loop;
		 }
		 else
		 {
			 //产生冲突（Bus!=ID），报告"xxx send collision"
			 printf("%d send collision\n",ID1);
			 Bus=0;
			 CollisionCounter--;//冲突次数+1
			 if(CollisionCounter>0)
			 {
				 //延迟重发，延迟算法采用截断二进制指数退避算法
				 Sleep(randNum*(int)pow(2,(CollisionCounter>10)?10:CollisionCounter)*collisionWindow);
				 goto Loop;
			 }
			 else
				 //数据发送失败（冲突计数器值为0）后，报告"xxx send failure"
				 printf("%d send failuer\n",ID1);
		 }
	 }
	 else
		 goto Loop;
	 return 0;
}


UINT bThread(LPVOID pParam)
{
	int j=0;//发送成功次数
	int CollisionCounter=16;//冲突计数器初始值为16
	//随机延迟算法中的冲突窗口值取0.005
	double collisionWindow=0.005;
	int randNum=rand()%3;
Loop:if(Bus==0)//总线空闲
	 {
		 Sleep(2);
		 Bus=Bus|ID2;//模拟发送包
		 Sleep(3);
		 if(Bus==ID2)//若判断为无冲突
		 {
			 //数据发送成功（Bus==ID），报告"xxx send success"
			 printf("%d send success\n",ID2);
			 Bus=0;//内存清零
			 CollisionCounter=16;//复原冲突计数器
			 Sleep(rand()%10);//经过一个随即延时
			 j++;
			 //在主机发送成功次数增加后，报告已发送成功的次数
			 printf("主机b成功发送%d次\n",j);
			 if(j<5)//每台主机须向总线上成功发送5次数据
				 goto Loop;
		 }
		 else
		 {
			  //产生冲突（Bus!=ID），报告"xxx send collision"
			 printf("%d send collision\n",ID2);
			 Bus=0;
			 CollisionCounter--;
			 if(CollisionCounter>0)
			 {
				 //延迟重发，延迟算法采用截断二进制指数退避算法
				 Sleep(randNum*(int)pow(2,(CollisionCounter>10)?10:CollisionCounter)*collisionWindow);
				 goto Loop;
			 }
			 else
				 //数据发送失败（冲突计数器值为0），报告"xxx send failure"
				 printf("%d send failuer\n",ID2);
		 }
	 }
	 else
		 goto Loop;
	 return 0;
}


