#pragma once

template <typename DLLInterfaceClass>
class CDLLLoader
{
public:

	CDLLLoader()
		: m_DLLHandle(NULL)
		, m_Interface(NULL)
	{
	}

	~CDLLLoader()
	{
		UnloadDLL();
	}

	DLLInterfaceClass* GetInterface()
	{
		if (!this->m_Interface)
		{
			if (LoadDLL()) return this->m_Interface;
			else return NULL;
		}
		else return NULL;
	}

	bool LoadDLL()
	{
		if (this->m_DLLHandle)
		{
			// Either this function has already been called or something else has assigned a value to the pointer.
			return false; // ?? This could be an error or OK depending on how this function is used.
		}

		this->m_DLLHandle = LoadLibrary("genesis_network.dll");
		
		if (!this->m_DLLHandle)
		{
			//fprintf(stderr, "Error loading DLL\n");
			return false;
		}

		// exported dll function pointer type
		typedef DLLInterfaceClass* (*PF_GetDLLInterface)();
		PF_GetDLLInterface pfunc = (PF_GetDLLInterface)GetProcAddress(this->m_DLLHandle, "GetDLLInterface");

		if (!pfunc)
		{
			//fprintf(stderr, "Error getting address of function\n");
			UnloadDLL();
			return false;
		}

		this->m_Interface = pfunc();

		if (!this->m_Interface)
		{
			//fprintf(stderr, "Error getting address of class\n");
			UnloadDLL();
			return false;
		}

		return true;
	}

	void UnloadDLL()
	{
		if (!this->m_DLLHandle)	return;
		//printf("Unloading DLL\n");
		FreeLibrary(this->m_DLLHandle);
		this->m_DLLHandle = NULL;
		this->m_Interface = NULL;
	}

	bool IsLoaded() const
	{
		return NULL != this->m_DLLHandle;
	}

private:
	HMODULE m_DLLHandle;
	DLLInterfaceClass* m_Interface;
};
