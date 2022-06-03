//��Ŀ:ģ����������̫������֡���͹���

#include "stdafx.h"
#include "course_design1.h"
#include "cmath"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//���������ӵļ��������Ϊ2����֧��CSMA\CDЭ��
CWinThread *thread1,*thread2;
//�������߳�a��bģ����̫���ϵ���̨��������һ��˫�����ͱ���Busģ�����ߡ�
DWORD ID1,ID2,Bus=0;
UINT aThread(LPVOID pParam);
UINT bThread(LPVOID pParam);

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ��MFC
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		thread1=AfxBeginThread(aThread,NULL);//�����߳�a
		ID1=thread1->m_nThreadID;
		thread2=AfxBeginThread(bThread,NULL);//�����߳�b
		ID2=thread2->m_nThreadID;
		getchar();//���̵߳ȴ�����������˳�����
	}

	return nRetCode;
}

UINT aThread(LPVOID pParam)
{
	int i=0;//���ͳɹ�����
	int CollisionCounter=16;//��ͻ��������ʼֵΪ16
	//����ӳ��㷨�еĳ�ͻ����ֵȡ0.005
	double collisionWindow=0.005;
	int randNum=rand()%3;
Loop:if(Bus==0)//���߿���
	 {
		 Bus=Bus|ID1;//ģ�ⷢ�Ͱ�
		 Sleep(2);
		 if(Bus==ID1)//���ж�Ϊ�޳�ͻ
		 {
			 //���ݷ��ͳɹ���Bus==ID��������"xxx send success"
			 printf("%d send success\n",ID1);
			 Bus=0;//�ڴ�����
			 CollisionCounter=16;//��ԭ��ͻ������
			 Sleep(rand()%10);//����һ���漴��ʱ
			 i++;
			 //���������ͳɹ��������Ӻ󣬱����ѷ��ͳɹ��Ĵ���
			 printf("����a�ɹ�����%d��\n",i);
			 if(i<5)//ÿ̨�������������ϳɹ�����5������
				 goto Loop;
		 }
		 else
		 {
			 //������ͻ��Bus!=ID��������"xxx send collision"
			 printf("%d send collision\n",ID1);
			 Bus=0;
			 CollisionCounter--;//��ͻ����+1
			 if(CollisionCounter>0)
			 {
				 //�ӳ��ط����ӳ��㷨���ýض϶�����ָ���˱��㷨
				 Sleep(randNum*(int)pow(2,(CollisionCounter>10)?10:CollisionCounter)*collisionWindow);
				 goto Loop;
			 }
			 else
				 //���ݷ���ʧ�ܣ���ͻ������ֵΪ0���󣬱���"xxx send failure"
				 printf("%d send failuer\n",ID1);
		 }
	 }
	 else
		 goto Loop;
	 return 0;
}


UINT bThread(LPVOID pParam)
{
	int j=0;//���ͳɹ�����
	int CollisionCounter=16;//��ͻ��������ʼֵΪ16
	//����ӳ��㷨�еĳ�ͻ����ֵȡ0.005
	double collisionWindow=0.005;
	int randNum=rand()%3;
Loop:if(Bus==0)//���߿���
	 {
		 Sleep(2);
		 Bus=Bus|ID2;//ģ�ⷢ�Ͱ�
		 Sleep(3);
		 if(Bus==ID2)//���ж�Ϊ�޳�ͻ
		 {
			 //���ݷ��ͳɹ���Bus==ID��������"xxx send success"
			 printf("%d send success\n",ID2);
			 Bus=0;//�ڴ�����
			 CollisionCounter=16;//��ԭ��ͻ������
			 Sleep(rand()%10);//����һ���漴��ʱ
			 j++;
			 //���������ͳɹ��������Ӻ󣬱����ѷ��ͳɹ��Ĵ���
			 printf("����b�ɹ�����%d��\n",j);
			 if(j<5)//ÿ̨�������������ϳɹ�����5������
				 goto Loop;
		 }
		 else
		 {
			  //������ͻ��Bus!=ID��������"xxx send collision"
			 printf("%d send collision\n",ID2);
			 Bus=0;
			 CollisionCounter--;
			 if(CollisionCounter>0)
			 {
				 //�ӳ��ط����ӳ��㷨���ýض϶�����ָ���˱��㷨
				 Sleep(randNum*(int)pow(2,(CollisionCounter>10)?10:CollisionCounter)*collisionWindow);
				 goto Loop;
			 }
			 else
				 //���ݷ���ʧ�ܣ���ͻ������ֵΪ0��������"xxx send failure"
				 printf("%d send failuer\n",ID2);
		 }
	 }
	 else
		 goto Loop;
	 return 0;
}


