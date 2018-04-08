// 2017_GetPE_IMAGE_EXPORT_DIRECTORY.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>


//��ȡPE�ļ�������
//���˼·��
//1�����ļ�����ȡ�ļ����
//2����ȡ�ļ����ڴ滺����
//3����ȡDOSͷ�ṹ
//4����ȡNTͷ�ṹ
//5����ȡ��չͷ�ṹ
//6����ȡ����Ŀ¼��
//7����ȡ������
//8����װ��������ַת���ļ�ƫ�Ƶ�ַ����



/////////////////////////////////////////
//          RVAתFOA����               //
/////////////////////////////////////////
DWORD   RvaToOffset(const void* pFileData, DWORD dwRva)
{

	// ��ȡDOSͷ
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)pFileData;

	// ��ȡNTͷ
	IMAGE_NT_HEADERS *pNtHeader = (IMAGE_NT_HEADERS*)((DWORD)pDosHeader + pDosHeader->e_lfanew);

	// �õ����θ���
	DWORD   dwSectionNumber = pNtHeader->FileHeader.NumberOfSections;

	// �õ�����
	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);

	// �������α��ҵ�RVA���ڵ�����
	/*
	* ÿ��ƫ�ƣ����������ļ��У��������ڴ��У����Ǿ������ο�ʼλ�õľ���
	* ������ȵġ�
	* ���ң����α��У�������������ʼƫ�ƣ�
	*  1. �ļ��еĿ�ʼƫ��
	*  2. �ڴ��еĿ�ʼƫ��
	* ������̣�
	*  �ҵ�RVA�������Σ� Ȼ���������RVA���������ڴ��еĿ�ʼλ�õľ��롣
	*  �������������������ļ��еĿ�ʼλ�þ͵õ��ļ�ƫ����
	*/

	for (int i = 0; i < dwSectionNumber; ++i) {

		// ���ε���ʼ��������ַRVA
		DWORD dwSectionBeginRva = pSectionHeader[i].VirtualAddress;

		// ����Ľ�����������ַRVA = ���ε�RVA��ַ + �ļ��е����ζ����С
		DWORD dwSectionEndRva = pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;


		// �ж�RVA�Ƿ��ڵ�ǰ��������
		if (dwRva >= dwSectionBeginRva
			&& dwRva <= dwSectionEndRva) {

			// �����RVA��Ӧ���ļ�ƫ��
			// ��ʽ���ļ�ƫ��  =  RVA - ���ε���ʼ��������ַRVA + ���ε���ʼ�ļ�ƫ��FOA
			// 1. Ҫת����RVA - ���ε���ʼ��������ַRVA
			DWORD dwTemp = dwRva - pSectionHeader[i].VirtualAddress;
			// 2. �������ε���ʼ�ļ�ƫ��FOA��dwOffsetΪFOA
			DWORD dwOffset = dwTemp + pSectionHeader[i].PointerToRawData;
			// 3. �õ��ļ�ƫ��FOA
			return dwOffset;
		}
	}

	return -1;
}


int main()
{
	// Ҫ������PE�ļ�
	//char dllPath[MAX_PATH] = "D:\\PEDemo.dll";
	//char dllPath[MAX_PATH] = "D:\\PEDemofnPEDemoFunB@2fnPEDemoFunA@3.dll";
	char dllPath[MAX_PATH] = "D:\\PEDemofnPEDemoFunB@2NONAME.dll";
	//char dllPath[MAX_PATH] = "D:\\PEDemofnPEDemoFunB@2private.dll";


	// ��ȡPE�ļ�
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFileA(dllPath,   //PE�ļ�·��
		GENERIC_READ,              //�ļ����ʵ�Ȩ�ޣ�ͨ����GENERIC_READ, GENERIC_WRITE
		FILE_SHARE_READ,           //���ļ��Ĳ�����ʽ
		NULL,                      //��ȫ������
		OPEN_EXISTING,             //�Դ��ڵ��ļ����õĲ���
		FILE_ATTRIBUTE_NORMAL,     //�ļ����豸���Ժͱ�־
		NULL);                     //Ϊ�������ļ��ṩ�ļ����Ժ���չ����

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("�ļ������ڣ����߱�ռ��\n");
		return 0;
	}

	// ��ȡ�ļ���С
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// ���뻺���������ļ�����
	BYTE* pFileData = new BYTE[dwFileSize];
	DWORD dwRead = 0;
	// ���ļ���ȡ��������
	ReadFile(hFile, pFileData, dwFileSize, &dwRead, NULL);

	// ʹ��DOSͷ�ṹָ�򻺳���
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)pFileData;

	// ��ȡIMAGE_NT_HEADERS��NTͷ
	IMAGE_NT_HEADERS* pNtHeader = (IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)pFileData);

	// ��ȡIMAGE_OPTIONAL_HEADER32����չͷ
	IMAGE_OPTIONAL_HEADER* pOptionHeader = (IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;

	// ��ȡIMAGE_DATA_DIRECTORIES������Ŀ¼�� 
	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;

	// ��ȡIMAGE_DATA_DIRECTORY.Export������Ŀ¼��.������
	DWORD dwExportTableRva = pDataDirectory[0].VirtualAddress;

	// RVAתFOA��IMAGE_DATA_DIRECTORY.Exportת�ļ�ƫ��
	DWORD dwExportTableOffset = RvaToOffset(pFileData, dwExportTableRva);

	////////////////////////////////////////////////////////////
	// ��ȡ������IMAGE_EXPORT_DIRECTORY�ṹ���ļ�ƫ��λ�ã������ǽṹ�庬�壺
	//IMAGE_EXPORT_DIRECTORY{
	//    DWORD   Characteristics;        // 1)  ��������Ϊ0x00000000
	//    DWORD   TimeDateStamp;          // 2)  ʱ���������������ʱ�䣨GMTʱ�䣩
	//    WORD MajorVersion;              // 3)  ���汾�ţ�����������汾��
	//    WORDMinorVersion;               // 4)  �Ӱ汾�ţ���������Ӱ汾��
	//    DWORD   Name;                   // 5)  (����)ָ��ģ�����Ƶ�RVA��ָ��ģ����������������ģ������ƣ���ASCII�ַ���RVA
	//    DWORD   Base;                   // 6)  (����)�������������API��������ֵ�Ļ�������������ֵ=������������ֵ-������
	//    DWORD   NumberOfFunctions;      // 7)  (����)EAT ������ַ���еĳ�Ա����
	//    DWORD   NumberOfNames;          // 8)  (����)ENT �������Ʊ��еĳ�Ա����
	//    DWORD   AddressOfFunctions;     // 9)  (����)EAT ������ַ�����������ַ(RVA),ÿһ����0�����Ӧһ���������ĺ������ƻ����
	//    DWORD   AddressOfNames;         // 10) (����)ENT �������Ʊ����������ַ(RVA)��ÿһ����0�����Ӧһ���������ĺ�����ַ�����
	//    DWORD  AddressOfNameOrdinals;   // 11) (����)ָ�򵼳��������кŵ����飬������ű����������ַ(RVA)
	//} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;
	////////////////////////////////////////////////////////////
	IMAGE_EXPORT_DIRECTORY* pExportTable = /*����*/
		(IMAGE_EXPORT_DIRECTORY*)(dwExportTableOffset + (DWORD)pFileData);

	//������õ���ֵ
	printf("IMAGE_EXPORT_DIRECTORY.Name��0x%08X\n", pExportTable->Name);                            // ģ������
	printf("IMAGE_EXPORT_DIRECTORY.Base��0x%08X\n", pExportTable->Base);                            // ��������ֵ�Ļ���
	printf("IMAGE_EXPORT_DIRECTORY.NumberOfFunctions��0x%08X\n", pExportTable->NumberOfFunctions);  // ������ַ���еĳ�Ա����
	printf("IMAGE_EXPORT_DIRECTORY.NumberOfNames��0x%08X\n", pExportTable->NumberOfNames);          // �������Ʊ��еĳ�Ա����
	printf("IMAGE_EXPORT_DIRECTORY.AddressOfFunctions��0x%08X\n", pExportTable->AddressOfFunctions);// ������ַ�����������ַ(RVA)
	printf("IMAGE_EXPORT_DIRECTORY.AddressOfNames��0x%08X\n", pExportTable->AddressOfNames);        // �������Ʊ����������ַ(RVA)
	printf("IMAGE_EXPORT_DIRECTORY.AddressOfNameOrdinals��0x%08X\n", pExportTable->AddressOfNameOrdinals); // ָ�򵼳��������кŵ�����


	// IMAGE_EXPORT_DIRECTORY.Nameת��ΪFOA����ȡָ�򵼳����ļ������ַ���
	DWORD dwNameOffset = RvaToOffset(pFileData, pExportTable->Name);

	// IMAGE_EXPORT_DIRECTORY.Baseת��ΪFOA����ȡ����
	DWORD dwBaseOffset = RvaToOffset(pFileData, pExportTable->Base);

	// IMAGE_EXPORT_DIRECTORY.AddressOfFunctionsת��ΪFOA����ȡ����������ַ��
	DWORD dwAddressOfFunctionsOffset = RvaToOffset(pFileData, pExportTable->AddressOfFunctions);

	// IMAGE_EXPORT_DIRECTORY.AddressOfNamesת��ΪFOA����ȡ�����������Ƶ�ַ��
	DWORD dwAddressOfNamesOffset = RvaToOffset(pFileData, pExportTable->AddressOfNames);

	// IMAGE_EXPORT_DIRECTORY.AddressOfNameOrdinalsת��ΪFOA����ȡ����������ű�
	DWORD dwAddressOfNameOrdinalsOffset = RvaToOffset(pFileData, pExportTable->AddressOfNameOrdinals);

	//  IMAGE_EXPORT_DIRECTORY.Name ָ�򵼳���Name�ֶΣ����ݴ洢����ģ�麯�����Ƶ�ASCII�ַ�
	char* pDllName = (char*)(dwNameOffset + (DWORD)pFileData);
	printf("\nDll_Name: %s\n", pDllName);

	/////////////////////////////////////////
	// �����еĵ����ĺ�����ַ��ӡ������
	// ���ң�����������Ƶ����������������
	// ���������ŵ��������������š�
	/////////////////////////////////////////

	// IMAGE_EXPORT_DIRECTORY.AddressOfFunctionsָ�򵼳�������ַ��
	DWORD* pAddressTable = /*����*/
		(DWORD*)((DWORD)pFileData + dwAddressOfFunctionsOffset);

	// IMAGE_EXPORT_DIRECTORY.AddressOfNameOrdinalsָ�򵼳�������ű�
	// WORD* pOrdinalTable =(WORD*)((DWORD)pFileData + RvaToOffset(pFileData, pExportTable->AddressOfNameOrdinals));
	WORD* pOrdinalTable = (WORD*)((DWORD)pFileData + dwAddressOfNameOrdinalsOffset);

	// �����������ƣ���Ҫ��ת��һ��RVA�����ܵõ������������ڵ�λ��
	DWORD* pNameTable =(DWORD*)((DWORD)pFileData + dwAddressOfNamesOffset);

	// �ж�������ŵ����������Ժ���������
	BOOL bIndexIsExist = FALSE;

	//////////////////////////////////////////////////////////////////////////////////////
	/// pExportTable->NumberOfFunctions ��Ӧ��IMAGE_EXPORT_DIRECTORY.NumberOfFunctions ///
	/// ������ַ���еĳ�Ա����                                                         ///
	//////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < pExportTable->NumberOfFunctions; ++i) {

		// ��ӡ����š�����������ַ��(RVA)
		printf("�����[%d] ", i);
		printf("��ַ(RVA)�� %08X", pAddressTable[i]);

		// �жϵ�ǰ�������ַ�Ƿ��������Ʒ�ʽ������
		// �ж����ݣ�
		//   ��ű�����ǵ�ַ���һ���±꣬����±��¼��
		//   ��ַ�����ĸ���ַ�������Ʒ�ʽ�����ġ�
		//   �����ǰ������±걣������ű��У���˵�������ַ
		//   ��һ�����Ʒ�ʽ�������������±�����ű��в����ڣ�
		//   ��˵���������ַ����һ�����Ʒ�ʽ��������������Ž��е���
		bIndexIsExist = FALSE;


		// �Ե������Ƶ����ĺ�������������ѭ��
		int nNameIndex = 0;
		for (; nNameIndex < pExportTable->NumberOfNames; ++nNameIndex) {

			// �жϵ�ַ����±��Ƿ��������ű���
			if (i == pOrdinalTable[nNameIndex]) {
				bIndexIsExist = TRUE;
				break;
			}
		}

		// �ж����bIndexIsExistΪ����Ǻ����������������Ժ�����ŵ�����
		// ������Ҫ��ת��һ��RVA
		if (bIndexIsExist == TRUE) {

			// �õ����Ʊ��е�RVA
			DWORD dwNameRva = pNameTable[nNameIndex];

			// ������Rvaת���ɴ�����ʵ�������Ƶ��ļ�ƫ��
			char* pFunName =
				(char*)((DWORD)pFileData + RvaToOffset(pFileData, dwNameRva));

			printf(" ������:��%s��\t", pFunName);
			// i �� �ǵ�ַ���е������ţ�Ҳ����һ�������
			// ��������� = ����� + ��Ż���
			printf(" ���:��%d�� ", i + pExportTable->Base);
		}
		// ��û�е����������ƣ���������Ž��е���ʹ��
		if (bIndexIsExist == FALSE)
		{

			// �жϵ�ַ��ǰ��������Ԭ���Ƿ񱣴��ŵ�ַ
			if (pAddressTable[i] != 0) {

				printf(" ������:��-��\t");
				// i �� �ǵ�ַ���е������ţ�Ҳ����һ�������
				// ��������� = ����� + ��Ż���
				printf(" ���:��%d��", i + pExportTable->Base);
			}
		}

		printf("\n");
	}

	system("pause");
	return 0;
}