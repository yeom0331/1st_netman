
// NetmanDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Netman.h"
#include "NetmanDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetmanDlg 대화 상자



CNetmanDlg::CNetmanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NETMAN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetmanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_niclist);
	DDX_Control(pDX, IDC_LIST2, m_iplist);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Control(pDX, IDC_IPADDRESS2, m_sub);
}

BEGIN_MESSAGE_MAP(CNetmanDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CNetmanDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CNetmanDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CNetmanDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CNetmanDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CNetmanDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CNetmanDlg 메시지 처리기

BOOL CNetmanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_niclist.InsertColumn(1, _T("NAME"), LVCFMT_CENTER, 340, -1);
	m_niclist.InsertColumn(2, _T("DESCRIPTION"), LVCFMT_CENTER, 300, -1);
	m_niclist.InsertColumn(3, _T("IP"), LVCFMT_CENTER, 100, -1);
	m_niclist.InsertColumn(4, _T("SUBNET MASK"), LVCFMT_CENTER, 150, -1);

	//m_iplist.InsertColumn(0, _T("check"), LVCFMT_CENTER, 50, -1);
	m_iplist.InsertColumn(0, _T("IP"), LVCFMT_CENTER, 220, -1);
	m_iplist.InsertColumn(1, _T("SUBNET MASK"), LVCFMT_CENTER, 220, -1);

	setNicList();
	//dev_info();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CNetmanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CNetmanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNetmanDlg::setNicList()
{
	pcap_if_t* Devices, *d;
	pcap_addr_t* a;
	ULONG buffer_len = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
	int ret = pcap_findalldevs(&Devices, errbuf);
	if (ret == -1)
	{
		printf("pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}
	UpdateData(TRUE);
	int index = m_niclist.GetItemCount();
	int i=0;
	for (d = Devices; d != NULL; d = d->next) {		
		m_niclist.InsertItem(index, d->name);
		m_niclist.SetItemText(index, 1, d->description);
		for (a = d->addresses; a; a = a->next) {
			switch (a->addr->sa_family) {
			case AF_INET:
				if (a->addr)
				{
					m_niclist.SetItemText(index, 2, inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr));
				}
				if (a->netmask)
				{
					m_niclist.SetItemText(index, 3, inet_ntoa(((struct sockaddr_in*)a->netmask)->sin_addr));
				}
				break;
			}
		}
	}
	
}

void CNetmanDlg::setIpList() 
{

}
void CNetmanDlg::OnBnClickedButton2() //multi-delete
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nItem;
	POSITION pos;

	pos = m_iplist.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		nItem = m_iplist.GetNextSelectedItem(pos);
		m_iplist.DeleteItem(nItem);
		pos = m_iplist.GetFirstSelectedItemPosition();
	}
}


void CNetmanDlg::OnBnClickedButton5() //delete all
{
	m_iplist.DeleteAllItems();
}



void CNetmanDlg::OnBnClickedButton1() //add
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE ip_[4], sub_[4];
	CString strIpAddr, strSubAddr;
	int index = m_iplist.GetItemCount();
	m_ip.GetAddress(ip_[0], ip_[1], ip_[2], ip_[3]);
	m_sub.GetAddress(sub_[0], sub_[1], sub_[2], sub_[3]);
	strIpAddr.Format(_T("%d.%d.%d.%d"), ip_[0], ip_[1], ip_[2], ip_[3]);
	strSubAddr.Format(_T("%d.%d.%d.%d"), sub_[0], sub_[1], sub_[2], sub_[3]);
	
	m_iplist.InsertItem(index, strIpAddr);
	m_iplist.SetItemText(index, 1, strSubAddr);
}

USHORT CNetmanDlg::checksum(USHORT* buff, int size) {
	unsigned long cksum = 0;
	while (size > 1) {
		cksum += *buff++;
		size -= sizeof(USHORT);
	}
	if (size) {
		cksum += *(UCHAR*)buff;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum);
}


void CNetmanDlg::OnBnClickedButton3() //send_arp
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pcap_if_t* alldevs = NULL;
	char errbuf[PCAP_ERRBUF_SIZE];
	int offset = 0;
	if (pcap_findalldevs(&alldevs, errbuf) == -1) {
		AfxMessageBox(TEXT("dev find failed"));
	}
	if (alldevs == NULL) {
		AfxMessageBox(TEXT("no dev found"));
	}

	pcap_if_t* d; int i, inum;


	printf("enter the interface number: ");
	scanf("%d", &inum);
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++); // jump to the i-th dev

// open
	pcap_t* handle = pcap_open_live(d->name, BUFSIZ, 1, 1000, errbuf);
	if (handle == nullptr) {
		AfxMessageBox(TEXT("couldn't open device %s(%s)", d->name, errbuf));
		pcap_freealldevs(alldevs);
	}
	pcap_freealldevs(alldevs);

	EthArpPacket request_packet;
	request_packet.eth_.dmac_ = Mac("FF:FF:FF:FF:FF:FF");
	request_packet.eth_.smac_ = Mac("14:B3:1F:19:03:A4");
	request_packet.eth_.type_ = htons(EthHdr::Arp);

	request_packet.arp_.hrd_ = htons(ArpHdr::ETHER);
	request_packet.arp_.pro_ = htons(EthHdr::Ip4);
	request_packet.arp_.hln_ = Mac::SIZE;
	request_packet.arp_.pln_ = Ip::SIZE;
	request_packet.arp_.op_ = htons(ArpHdr::Request);

	request_packet.arp_.smac_ = Mac("14:B3:1F:19:03:A4");
	request_packet.arp_.sip_ = htonl(Ip("10.0.2.15"));
	request_packet.arp_.tmac_ = Mac("00:00:00:00:00:00");
	request_packet.arp_.tip_ = htonl(Ip("10.0.2.254"));

	int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(&request_packet), sizeof(EthArpPacket));
	if (res != 0) {
		AfxMessageBox(TEXT("pcap_sendpacket return %d error %s", res, pcap_geterr(handle)));
	}
}


void CNetmanDlg::OnBnClickedButton4() //send_icmp
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WSADATA wsaData;
	SOCKET s;
	SOCKADDR_STORAGE dest;
	icmp_header* icmp = NULL;
	char buf[sizeof(icmp_header) + 32];

	icmp = (icmp_header*)buf;
	icmp->icmp_type = 8;
	icmp->icmp_code = 0;
	icmp->icmp_id = (unsigned short)GetCurrentProcessId;
	icmp->icmp_checksum = 0;
	icmp->icmp_seq = 0;
	memset(&buf[sizeof(icmp_header)], '@', 32);
	icmp->icmp_checksum = checksum((USHORT*)buf, sizeof(icmp_header) + 32);

	s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (s == INVALID_SOCKET) {
		AfxMessageBox(TEXT("WSAStartup error!"));
	}

	((SOCKADDR_IN*)&dest)->sin_family = AF_INET;
	((SOCKADDR_IN*)&dest)->sin_port = htons(0);
	((SOCKADDR_IN*)&dest)->sin_addr.S_un.S_addr = inet_addr("8.8.8.8");

	for (int i = 0; i < 4; i++) {
		sendto(s, buf, sizeof(icmp_header) + 32, 0, (SOCKADDR*)&dest, sizeof(dest));
		Sleep(100);
	}
	closesocket(s);
	WSACleanup();
}

/*void CNetmanDlg::dev_info() { //get local mac, ip
	PIP_ADAPTER_INFO AdapterInfo;
	CString ip_ ,mac_;
	DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
	int index = m_niclist.GetItemCount();
	char* mac_addr = (char*)malloc(18);

	AdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if (AdapterInfo == NULL) {
		AfxMessageBox(TEXT("Error allocating memory needed to call GetAdaptersinfo"));
		free(mac_addr);
		return; // it is safe to call free(NULL)
	}

	// Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(AdapterInfo);
		AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
		if (AdapterInfo == NULL) {
			AfxMessageBox(TEXT("Error allocating memory needed to call GetAdaptersinfo"));
			free(mac_addr);
			return;
		}
	}

	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
		// Contains pointer to current adapter info
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
		do {
			// technically should look at pAdapterInfo->AddressLength
			//   and not assume it is 6.
			mac_.Format("%02x:02x:02x:02x:02x:02x",
				pAdapterInfo->Address[0],
				pAdapterInfo->Address[1],
				pAdapterInfo->Address[2],
				pAdapterInfo->Address[3],
				pAdapterInfo->Address[4],
				pAdapterInfo->Address[5]);
			// print them all, return the last one.
			// return mac_addr;

			//printf("\n");
			pAdapterInfo = pAdapterInfo->Next;
		} while (pAdapterInfo);

	}
	free(AdapterInfo);
}*/
