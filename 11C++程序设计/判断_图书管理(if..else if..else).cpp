#include <iostream>
using namespace std;

/*ѧУ��ͼ�����ϵͳһ�������¶�����ܣ�
���顢���顢ͼ��������ͼ�������Ϣ������ǰͼ��״̬��ѯ�ȵ�*/
int main()
{
	cout<<"ͼ�����ϵͳ���ܣ�"<<endl;
	cout<<"1-���鹦�ܹ���"<<endl;
	cout<<"2-���鹦�ܹ���"<<endl;
	cout<<"3-ͼ��������"<<endl;
	cout<<"4-ͼ�������Ϣ����"<<endl;
	cout<<"5-��ǰͼ��״̬��ѯ����"<<endl;
	cout<<"0-�˳�"<<endl;
	int status;
	int count = 10;

	while(count!=0)
	{
		cout<<"����������ǰ��ѡ��";
		cin>>status;

		if(status == 0)//�˳�
		{
			cout<<"�˳��ɹ�!"<<endl;
			break;
		}
		else if(status == 1)//����
			cout<<"������鹦�ܹ���ģ��!"<<endl;
		else if(status == 2)//����
			cout<<"���뻹�鹦�ܹ���ģ��!"<<endl;
		else if(status == 3)//ͼ��������
			cout<<"����ͼ��������ģ��!"<<endl;
		else if(status == 4)//ͼ�������Ϣ
			cout<<"����ͼ�������Ϣ����ģ��!"<<endl;
		else//��ѯ��ǰͼ��״̬
			cout<<"���뵱ǰͼ��״̬��ѯ����!"<<endl;
		count--;
	}

	return 0;
}