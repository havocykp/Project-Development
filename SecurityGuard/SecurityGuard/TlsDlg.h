#pragma once
#include "Pe.h"


// CTlsDlg 对话框

class CTlsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTlsDlg)

public:
	CTlsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTlsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CPe* m_pPE;
	CString m_StartAddressOfRawData;
	CString m_EndAddressOfRawData;
	CString m_AddressOfIndex;
	CString m_AddressOfCallBacks;
	CString m_SizeOfZeroFill;
	CString m_Characteristics;
};
