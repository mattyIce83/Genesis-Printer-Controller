// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CSCSkin3 wrapper class

class CSCSkin3 : public COleDispatchDriver
{
public:
	CSCSkin3(){} // Calls COleDispatchDriver default constructor
	CSCSkin3(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSCSkin3(const CSCSkin3& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// ISCSkin3 methods
public:
	void InitDecoration(long mode)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void LoadSkinFromFile(LPCTSTR path)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, path);
	}
	void DoNotDecorate(long hWnd)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd);
	}
	void DoDecorate(long hWnd)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd);
	}
	void IncludeWnd(long hWnd, long withchildren)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd, withchildren);
	}
	void ExcludeWnd(long hWnd, long withchildren)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd, withchildren);
	}
	void DeInitDecoration()
	{
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DecorateAs(long hWnd, long type)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd, type);
	}
	void UpdateControl(long nID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nID);
	}
	void InitLicenKeys(LPCTSTR reg_name, LPCTSTR company, LPCTSTR email, LPCTSTR licenkey)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, reg_name, company, email, licenkey);
	}
	void UpdateWindow(long hWnd)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd);
	}
	void RemoveSkin()
	{
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DefineLanguage(long langID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, langID);
	}
	void ApplySkin()
	{
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GetSkinCopyRight(LPCTSTR skinpath, BSTR * name, BSTR * author, BSTR * date, BSTR * email)
	{
		static BYTE parms[] = VTS_BSTR VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PBSTR ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, skinpath, name, author, date, email);
	}
	void AboutSkinCrafter()
	{
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetCustomSkinWnd(long hWnd, LPCTSTR skinName, long isFrame)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd, skinName, isFrame);
	}
	void AddAdditionalThread()
	{
		InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DeleteAdditionalThread()
	{
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AddSkinFromFile(LPCTSTR path, short sID)
	{
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms, path, sID);
	}
	void ApplyAddedSkin(long hWnd, short sID)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd, sID);
	}
	void RemoveAddedSkin(short sID)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sID);
	}
	void SetCustomScrollbars(long hWnd, LPCTSTR skinName)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd, skinName);
	}
	void SetAddedCustomScrollbars(long hWnd, short sID, LPCTSTR skinName)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_BSTR ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd, sID, skinName);
	}
	void SetAddedCustomSkinWnd(long hWnd, short sID, LPCTSTR skinName, long isFrame)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_BSTR VTS_I4 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd, sID, skinName, isFrame);
	}
	void LoadSkinFromResource(long hModule, long hrsrcResInfo)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hModule, hrsrcResInfo);
	}
	void GetUserDataSize(LPCTSTR skinName, long lptrSize)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, skinName, lptrSize);
	}
	void GetUserData(LPCTSTR skinName, long lptrData, long lDataSize)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, skinName, lptrData, lDataSize);
	}
	void DeleteAddedSkin(short sID)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sID);
	}
	void SetDecorationMode(long DecorationMode)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DecorationMode);
	}
	void IncludeThreadWindows(long lThreadId, long withchildren)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lThreadId, withchildren);
	}
	void ExcludeThreadWindows(long lThreadId, long withchildren)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lThreadId, withchildren);
	}
	void ClearSkin()
	{
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ClearWnd(long hWnd, long withchildren)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd, withchildren);
	}
	void BeginHSL(long type, long hslID)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, type, hslID);
	}
	void ModifyHSL(long hslID, double hue, double saturation, double lightness, double opacity)
	{
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hslID, hue, saturation, lightness, opacity);
	}
	void EndHSL(long hslID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hslID);
	}
	void BeginCustomHSL(LPCTSTR skinName, long hslID)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, skinName, hslID);
	}
	void GetAddedUserDataSize(short sID, LPCTSTR skinName, long lptrSize)
	{
		static BYTE parms[] = VTS_I2 VTS_BSTR VTS_I4 ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sID, skinName, lptrSize);
	}
	void GetAddedUserData(short sID, LPCTSTR skinName, long lptrData, long lDataSize)
	{
		static BYTE parms[] = VTS_I2 VTS_BSTR VTS_I4 VTS_I4 ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sID, skinName, lptrData, lDataSize);
	}
	void AddAdditionalThreadById(long lId)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lId);
	}
	void DeleteAdditionalThreadById(long lId)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lId);
	}
	void SetAlphaAnimation(long hWnd, short nWndType, short nNumFrames, short nSleepInterval)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I2 ;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hWnd, nWndType, nNumFrames, nSleepInterval);
	}

	// ISCSkin3 properties
public:

};
