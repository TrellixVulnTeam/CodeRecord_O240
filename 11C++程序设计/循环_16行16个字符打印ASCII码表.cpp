#include <iostream>
using namespace std;

int main()
{
    int i =0;
    for(int rows = 0; rows < 16; rows++)
    {

        i = rows;                             // 16�е��ַ�
        while(i <= 127)
        {
			switch(i)
			{
			case 7://����
				cout<<i<<"="<<"\\a"<<" | ";
				break;
			case 8://�˸�
				cout<<i<<"="<<"\\b"<<" | ";
				break;
			case 9://ˮƽ�Ʊ��
				cout<<i<<"="<<"\\t"<<" | ";
				break;
			case 10://����
				cout<<i<<"="<<"\\n"<<" | ";
				break;
			case 11://��ֱ�Ʊ��
				cout<<i<<"="<<"\\v"<<" | ";
				break;
			case 12://��ҳ
				cout<<i<<"="<<"\\f"<<" | ";
				break;
			case 13://�س�
				cout<<i<<"="<<"\\r"<<" | ";
				break;
			default:
				cout<<i<<"="<<char(i)<<" | ";  //������ת�����ַ����������char����
				break;
			}

            i+=16;//ÿ��16������һ��
        }

        cout<<endl;
    }

    return 0;
}