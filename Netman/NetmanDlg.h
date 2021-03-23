
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
	CWinThread* m_pthread;
	HANDLE hthread;
	bool m_run, m_stop;
	static UINT runthread(LPVOID pParam);
	void setNicList();
	CString mac_info();
	uint16_t calculate(uint16_t* buf, int size);
	uint16_t checksum(USHORT* buff, int size);
	uint16_t ip_checksum(uint8_t* buf);
	void get_mac(char *dev);
	void get_ip(char *dev);
	CListCtrl m_niclist;
	CListCtrl m_iplist;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	CIPAddressCtrl m_ip;
	CIPAddressCtrl m_sub;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	CEdit nic_choose;
	CEdit m_nicip;
	CEdit m_maclist;
};
