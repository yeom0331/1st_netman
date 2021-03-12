
// NetmanDlg.h: 헤더 파일
//

#pragma once


// CNetmanDlg 대화 상자
class CNetmanDlg : public CDialogEx
{
// 생성입니다.
public:
	CNetmanDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETMAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void setNicList();
	void setIpList();
	//void dev_info();
	USHORT checksum(USHORT* buf, int size);
	CListCtrl m_niclist;
	CListCtrl m_iplist;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	CIPAddressCtrl m_ip;
	CIPAddressCtrl m_sub;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
