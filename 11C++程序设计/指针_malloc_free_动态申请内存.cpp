#include <malloc.h>
#include <iostream>
using namespace std;

//073（动态申请内存）

//  当用指针指向一段内存时，实现并不知道需要申请多少，此时动态申请内存就显得很有必要。  //
//  																					//
//  （1）malloc方式																		//
//  当申请一维内存时候，格式如下：												 	    //
//  																					//
//  ```																					//
//  类型标识符 *变量名;																	//
//  变量名 = (类型标识符*)malloc(sizeof(类型标识符)*数组大小);						    //
//  ```																					//
//  																					//
//  在使用完该方式申请的内存后，必须用free()函数及时释放，格式如下：					//
//  																					//
//  ```																					//
//  free(变量名);																		//
//  变量名 = NULL;																		//
//  ```																					//
//  当申请二维内存时，格式如下：													    //
//  																					//
//  ```																					//
//  类型标识符 **变量名																	//
//  变量名 = (类型标识符**)malloc(sizeof(类型标识符*)*数组行大小);					    //
//  for (int i = 0; i < 数组行大小;i++)												    //
//      变量名[i]=(类型标识符*)malloc(sizeof(类型标识符)*数组列大小)    			    //
//  ```																					//
//  释放格式如下：																		//
//  ```																					//
//  free（变量名）;																	    //
//  变量名 = NULL;																		//
//  ```																					//



int main()  
{  
	int n;   //行,表示通信录中有几个联系人
	int m=2; //列，表示名字和其对应通信号
	cout<<"请输入要键入的通信录个数：";
	cin>>n;                                      //
	int  **memo;                                 // 通信录
	memo=(int**)malloc(sizeof(int *)*n);         // 申请内存
	for(int k=0;k<n;k++)                         
		memo[k]=(int *)malloc(sizeof(int)*m);  

	for(int i=0;i<n;i++)  
	{  
		for(int j=0;j<m;j++)  
			cin>>memo[i][j];                     //输入通信录内容
	}
	cout<<"通信录编号-------------通信号"<<endl;
	for(int i=0;i<n;i++)  
	{  
		for(int j=0;j<m;j++)  
			cout<<memo[i][j]<<"                   ";  
		cout<<endl;  
	}     
	free(memo);                                  //释放内存
	return 0;  
} 


//输出结果
//
//请输入要键入的通信录个数：5
//01 123
//02 456
//03 789
//04 567
//05 897
//通信录编号------------ - 通信号
//1                   123
//2                   456
//3                   789
//4                   567
//5                   897