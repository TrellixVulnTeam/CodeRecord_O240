// 20171105_bin2hex.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>

//Win32 + *nix
#ifdef WIN32
#define CRLF    "\r\n"
#else
#define CRLF    "\n"
#endif

//number of bytes per line
#define LINELEN     16

void printFileAsHex(FILE* file);
char toVisibleCharacter(int byteVal);
void writeLine(char* lineBuffer, int buflen);
char upperToHex(int byteVal);
char lowerToHex(int byteVal);
char nibbleToHex(int nibble);


int main(int argc, const char* argv[])
{
	FILE* file;

	//if (argc != 2)
	//{
	//	printf("Usage: phex <file>");
	//	printf(CRLF);
	//	return 1;
	//}

	// ��д�ļ�
	file = fopen("D:\\20171111-test.exe", "rb");  //���ļ�
	if (NULL == file)
	{
		printf("Cannot open %s", argv[1]);
		printf(CRLF);
		return 2;
	}

	printFileAsHex(file);

	fclose(file);

	system("pause");
	return 0;
}
//��Ҫʵ�ֺ���
void printFileAsHex(FILE* file)
{
	int count = 0;
	char buffer[LINELEN];                 //��ʱ����ַ�������

	while (1)
	{
		
		int byteVal = fgetc(file);        //ָ����ļ��ж�ȡһ���ַ�����ȡһ���ֽں󣬹��λ�ú���һ���ֽڡ�
		
		if (EOF == byteVal)               //����ļ��Ѿ���β������ѭ��
		{
			break;
		}
		
		buffer[count] = byteVal;          //���ļ��ַ��洢��������
		count++;                          //����ÿ���ַ�����Ŀ
		
		if (count >= LINELEN)             //��ǰ����ǰ16���ֽ�
		{
			writeLine(buffer, LINELEN);
			count = 0;
		}
	}

	if (count > 0)                           //����16�ֽڵ��������
	{
		writeLine(buffer, count);
	}
}

char toVisibleCharacter(int byteVal)
{
	if (byteVal >= 32 && byteVal <= 126)  // ����ɴ�ӡ�ַ�
	{
		return (char)byteVal;
	}

	return '_';                           //���ɴ�ӡ�����_��
}

void writeLine(char* lineBuffer, int buflen)
{
	int i;

	char str[LINELEN];

	// ��д�ļ�
	FILE* file = fopen("D:\\20171111-test.txt", "a+");

	for (i = 0; i < buflen; i++)
	{
		char chu = upperToHex(lineBuffer[i]); //ȡ��4λ  -->�����ʮ���Ƶ�ֵ
		char chl = lowerToHex(lineBuffer[i]); //ȡ��4λ  -->�����ʮ���Ƶ�ֵ
		printf("%c%c ", chu, chl);
		//�����ӵ�-д�ļ���txt
		sprintf(str, "%c%c", chu, chl);
		fseek(file, 0, SEEK_END);
		fwrite(str, strlen(str), 1, file);
	}
	fclose(file);

	if (buflen < LINELEN)
	{
		for (i = LINELEN - buflen; i > 0; i--)
		{
			printf("   ");
		}
	}

	printf("\t");



	char str1[LINELEN];
	for (i = 0; i < buflen; i++)
	{
		char ch = toVisibleCharacter(lineBuffer[i]);
		printf("%c", ch);
	}

	printf(CRLF);
}

char upperToHex(int byteVal)
{
	int i = (byteVal & 0xF0) >> 4;  //0F������Ϊ00001111������λ������Ȼ������4λ��Ҳ����ֻȡ����4λ����˼
	return nibbleToHex(i);
}

char lowerToHex(int byteVal)
{
	int i = (byteVal & 0x0F);  //������4λ00001111�����û�����ƣ����Ա�����4λ
	return nibbleToHex(i);
}

char nibbleToHex(int nibble)
{
	const int ascii_zero = 48;
	const int ascii_a = 65;

	if ((nibble >= 0) && (nibble <= 9))
	{
		return (char)(nibble + ascii_zero);   // ʮ������ת����ʮ���� �������֣�������int�ͣ�ǿת��char��
	}
	if ((nibble >= 10) && (nibble <= 15))
	{
		return (char)(nibble - 10 + ascii_a); // ʮ������ת����ʮ���Ƶ���ĸ��������int�ͣ�ǿת��char��
	}
	return '?';                               //�����������'?'
}

