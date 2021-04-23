
// NetmanDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Netman.h"
#include "NetmanDlg.h"
#include "thread"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
	DDX_Control(pDX, IDC_EDIT1, nic_choose);
	DDX_Control(pDX, IDC_EDIT2, m_nicip);
	DDX_Control(pDX, IDC_EDIT3, m_maclist);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_EDIT4, a);
	DDX_Control(pDX, IDC_EDIT5, b);
}

BEGIN_MESSAGE_MAP(CNetmanDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CNetmanDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CNetmanDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CNetmanDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CNetmanDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CNetmanDlg::OnBnClickedButton5)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CNetmanDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON6, &CNetmanDlg::OnBnClickedButton6)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CNetmanDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON7, &CNetmanDlg::OnBnClickedButton7)
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
	m_niclist.InsertColumn(2, _T("DESCRIPTION"), LVCFMT_CENTER, 340, -1);
	m_niclist.InsertColumn(3, _T("IP"), LVCFMT_CENTER, 160, -1);
	m_niclist.InsertColumn(4, _T("SUBNET MASK"), LVCFMT_CENTER, 160, -1);
	m_niclist.InsertColumn(5, _T("MAC ADDRESS"), LVCFMT_CENTER, 180, -1);

	//m_iplist.InsertColumn(0, _T("check"), LVCFMT_CENTER, 50, -1);
	m_iplist.InsertColumn(0, _T("IP"), LVCFMT_CENTER, 220, -1);
	m_iplist.InsertColumn(1, _T("COMMUNICATION"), LVCFMT_CENTER, 250, -1);
	m_iplist.InsertColumn(2, _T("ALLOCATION"), LVCFMT_CENTER, 250, -1);

	setNicList();

	m_combo.AddString(TEXT("16"));
	m_combo.AddString(TEXT("24"));
	m_combo.AddString(TEXT("32"));

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

//show niclist
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
		return;
	}
	UpdateData(TRUE);

	CString strMac;
	DWORD size = sizeof(PIP_ADAPTER_INFO);
	std::vector<CString> v;

	PIP_ADAPTER_INFO Info;
	int result = GetAdaptersInfo(Info, &size);        
	if (result == ERROR_BUFFER_OVERFLOW)
	{
		Info = (PIP_ADAPTER_INFO)malloc(size);
		GetAdaptersInfo(Info, &size);
	}

	if (!Info) return;
	do {
		strMac.Format("%0.2X:%0.2X:%0.2X:%0.2X:%0.2X:%0.2X",
			Info->Address[0], Info->Address[1], Info->Address[2], Info->Address[3], Info->Address[4], Info->Address[5]);
		v.push_back(strMac);
		Info = Info->Next;
	} while (Info);


	int index = m_niclist.GetItemCount();
	int i = 0;
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

			case AF_INET6:
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
		m_niclist.SetItemText(index, 4, v[i]);
		i++;
	}
}

//muti check delete 
void CNetmanDlg::OnBnClickedButton2()
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

//iplist delete all
void CNetmanDlg::OnBnClickedButton5()
{
	m_iplist.DeleteAllItems();
}


//add available ip in iplist
void CNetmanDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE ip_[4];
	CString strIpAddr;
	int index = m_iplist.GetItemCount();
	m_ip.GetAddress(ip_[0], ip_[1], ip_[2], ip_[3]);
	strIpAddr.Format(_T("%d.%d.%d.%d"), ip_[0], ip_[1], ip_[2], ip_[3]);

	if (ip_[0] == NULL || strIpAddr.IsEmpty()) {
		AfxMessageBox(_T("check your input!"));
		return;
	}

	m_iplist.InsertItem(index, strIpAddr);
	m_ip.ClearAddress();
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

//check available ip by arp and icmp reqeust
void CNetmanDlg::OnBnClickedButton3()
{
	BYTE ip_[4];
	CString strIpAddr;
	m_ip.GetAddress(ip_[0], ip_[1], ip_[2], ip_[3]);
	strIpAddr.Format(_T("%d.%d.%d.%d"), ip_[0], ip_[1], ip_[2], ip_[3]);

	if (ip_[0] == NULL) {
		AfxMessageBox(_T("check your input!"));
		return;
	}

	string s = string(CT2CA(strIpAddr));

	char errbuf[PCAP_ERRBUF_SIZE];
	CString dev, mac, ip;
	GetDlgItemText(IDC_EDIT1, dev);
	GetDlgItemText(IDC_EDIT2, ip);
	GetDlgItemText(IDC_EDIT3, mac);


	if (dev.IsEmpty()) {
		AfxMessageBox(("Choose the interface!"));
		return;
	}

	pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == nullptr) {
		AfxMessageBox(("couldn't open device %s(%s)", dev, errbuf));
		return;
	}

	EthArpPacket request_packet;

	int count = 0;
	request_packet.eth_.dmac_ = Mac("FF:FF:FF:FF:FF:FF");
	request_packet.eth_.smac_ = Mac((string)mac);
	request_packet.eth_.type_ = htons(EthHdr::Arp);

	request_packet.arp_.hrd_ = htons(ArpHdr::ETHER);
	request_packet.arp_.pro_ = htons(EthHdr::Ip4);
	request_packet.arp_.hln_ = Mac::SIZE;
	request_packet.arp_.pln_ = Ip::SIZE;
	request_packet.arp_.op_ = htons(ArpHdr::Request);

	request_packet.arp_.smac_ = Mac((string)mac);
	request_packet.arp_.sip_ = htonl(Ip((string)ip));
	request_packet.arp_.tmac_ = Mac("00:00:00:00:00:00");

	
	request_packet.arp_.tip_ = htonl(Ip(s));
	int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(&request_packet), sizeof(EthArpPacket));
	if (res != 0)
	{
		AfxMessageBox(("pcap_sendpacket return %d error %s", res, pcap_geterr(handle)));
		return;
	}

	Mac target;

	for (int i = 0; i < 20; i++) {
		struct pcap_pkthdr* header;
		const u_char* reply_packet;
		int res1 = pcap_next_ex(handle, &header, &reply_packet);
		if (res1 == 0) continue;
		if (res1 == -1 || res1 == -2)
		{
			printf("pcap_next_ex return %d(%s)\n", res1, pcap_geterr(handle));
			break;
		}

		struct EthArpPacket* etharp = (struct EthArpPacket*)reply_packet;
		if (etharp->eth_.type_ != htons(EthHdr::Arp)) continue;
		if (etharp->arp_.op_ != htons(ArpHdr::Reply)) continue;
		if (etharp->arp_.sip_ != request_packet.arp_.tip_) continue;
		target = etharp->arp_.smac_;
		count++;
		break;
	}

	if (count != 0) {
		MessageBox("Found ARP!");
		int nsize = sizeof(ethernet_header) + sizeof(ip_header) + sizeof(icmp_header);
		char* data = (char*)malloc(nsize);

		ethernet_header* eth = (ethernet_header*)data;
		eth->dmac_ = target;
		eth->smac_ = Mac((std::string)mac);
		eth->type_ = htons(EthHdr::Ip4);

		ip_header* iph = (ip_header*)(data + 14);
		iph->ip_hl = 0x45;
		iph->ip_tos = 0;
		iph->ip_len = ntohs(nsize);
		iph->ip_id = 0;
		iph->ip_off = 0;
		iph->ip_ttl = 128;
		iph->ip_pro = 1;
		iph->ip_chk = 0;
		iph->sip = htonl(Ip((std::string)ip));
		iph->dip = htonl(Ip(s));
		iph->ip_chk = checksum((USHORT*)iph, sizeof(ip_header));

		icmp_header* icmph = (icmp_header*)(data + 34);
		icmph->type = 8;
		icmph->code = 0;
		icmph->checksum = 0;
		icmph->id = htons(1);
		icmph->seq = htons(1);
		icmph->checksum = checksum((USHORT*)icmph, sizeof(icmp_header));


		int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(data), nsize);
		if (res != 0) {
			AfxMessageBox(("pcap_sendpacket return %d error %s", res, pcap_geterr(handle)));
			return;
		}

		for (int i = 0; i < 30; i++) {
			struct pcap_pkthdr* header;
			const u_char* icmp_reply;
			int res = pcap_next_ex(handle, &header, &icmp_reply);
			if (res == 0) continue;
			if (res == -1 || res == -2) {
				AfxMessageBox(("pcap_next_ex return %d(%s)", res, pcap_geterr(handle)));
				return;
			}

			struct ping_* icmpReply = (struct ping_*)icmp_reply;
			if (icmpReply->iph_.sip == iph->dip && icmpReply->icmp_.type == 0) 
			{
				MessageBox(("ICMP FOUND"));
				return;
			}
		}
		MessageBox(("Not Found ICMP Reply"));
	}
	else {
		MessageBox(("Cannot Found Arp Reply %s", strIpAddr));
	}
}

//check my network state by icmp request
void CNetmanDlg::OnBnClickedButton4() 
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다
	char errbuf[PCAP_ERRBUF_SIZE];
	CString dev, mac, ip;
	GetDlgItemText(IDC_EDIT1, dev);
	GetDlgItemText(IDC_EDIT2, ip);
	GetDlgItemText(IDC_EDIT3, mac);

	if (dev.IsEmpty()) {
		AfxMessageBox(("choose the interface!"));
		return;
	}

	pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == nullptr) {
		AfxMessageBox(("couldn't open device %s(%s)", dev, errbuf));
		return;
	}

	int nsize = sizeof(ethernet_header) + sizeof(ip_header) + sizeof(icmp_header);
	char* data = (char*)malloc(nsize);

	ethernet_header* eth = (ethernet_header*)data;
	eth->dmac_ = Mac("00:15:c6:3c:d4:42");
	eth->smac_ = Mac((std::string)mac);
	eth->type_ = htons(EthHdr::Ip4);

	ip_header* iph = (ip_header*)(data + 14);
	iph->ip_hl = 0x45;
	iph->ip_tos = 0;
	iph->ip_len = ntohs(nsize);
	iph->ip_id = 0;
	iph->ip_off = 0;
	iph->ip_ttl = 128;
	iph->ip_pro = 1;
	iph->ip_chk = 0;
	iph->sip = htonl(Ip((std::string)ip));
	iph->dip = htonl(Ip("8.8.8.8"));
	iph->ip_chk = checksum((USHORT*)iph, sizeof(ip_header));

	icmp_header* icmph = (icmp_header*)(data + 34);
	icmph->type = 8;
	icmph->code = 0;
	icmph->checksum = 0;
	icmph->id = htons(1);
	icmph->seq = htons(1);
	icmph->checksum = checksum((USHORT*)icmph, sizeof(icmp_header));


	int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(data), nsize);
	if (res != 0) {
		AfxMessageBox(("pcap_sendpacket return %d error %s", res, pcap_geterr(handle)));
		return;
	}

	for(int i=0; i<20; i++) {
		struct pcap_pkthdr* header;
		const u_char* icmp_reply;
		int res = pcap_next_ex(handle, &header, &icmp_reply);
		if (res == 0) continue;
		if (res == -1 || res == -2) {
			AfxMessageBox(("pcap_next_ex return %d(%s)", res, pcap_geterr(handle)));
			return;
		}

		struct ping_* icmpReply = (struct ping_*)icmp_reply;
		if (icmpReply->iph_.sip == iph->dip &&
			icmpReply->icmp_.type == 0) {
			MessageBox(("available network!"));
			return;
		}
	}
	MessageBox(("Non-available network!"));
}

uint32_t CNetmanDlg::IPToUInt(const std::string ip) {
	int a, b, c, d;
	uint32_t addr = 0;

	if (sscanf_s(ip.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
		return 0;

	addr = a << 24;
	addr |= b << 16;
	addr |= c << 8;
	addr |= d;
	return addr;
}

CString CNetmanDlg::ipToString(UINT32 ipaddr) {
	CString ipstr;
	ipstr.Format(_T("%d.%d.%d.%d"),
		(ipaddr >> 24) & 0xFF,
		(ipaddr >> 16) & 0xFF,
		(ipaddr >> 8) & 0xFF,
		(ipaddr >> 0) & 0xFF);
	return ipstr;
}

void CNetmanDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) //nic_list
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// GET SELECTED INFO
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;
	CString sIndexValue;
	sIndexValue = m_niclist.GetItemText(idx, 0);
	CString sMac = m_niclist.GetItemText(idx, 4);
	CString sIp = m_niclist.GetItemText(idx, 2);
	CString sub = m_niclist.GetItemText(idx, 3);
	SetDlgItemText(IDC_EDIT1, sIndexValue);
	SetDlgItemText(IDC_EDIT2, sIp);
	SetDlgItemText(IDC_EDIT3, sMac);
	SetDlgItemText(IDC_EDIT6, sub);
	*pResult = 0;
}

void send_arp(pcap_t* handle, Ip ip) {
	EthArpPacket request_packet;

	request_packet.eth_.dmac_ = Mac("FF:FF:FF:FF:FF:FF");
	request_packet.eth_.smac_ = Mac("14:B3:1F:19:03:A4");
	request_packet.eth_.type_ = htons(EthHdr::Arp);

	request_packet.eth_.type_ = htons(EthHdr::Arp);
	request_packet.arp_.hrd_ = htons(ArpHdr::ETHER);
	request_packet.arp_.pro_ = htons(EthHdr::Ip4);
	request_packet.arp_.hln_ = Mac::SIZE;
	request_packet.arp_.pln_ = Ip::SIZE;
	request_packet.arp_.op_ = htons(ArpHdr::Request);
	request_packet.arp_.smac_ = Mac("14:B3:1F:19:03:A4");
	request_packet.arp_.sip_ = htonl(Ip("10.0.2.212"));
	request_packet.arp_.tmac_ = Mac("00:00:00:00:00:00");
	request_packet.arp_.tip_ = htonl(ip);

	int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(&request_packet), sizeof(EthArpPacket));
	if (res != 0) {
		fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(handle));
	}
}


// arping in network
void CNetmanDlg::OnBnClickedButton6() 
{
	m_iplist.DeleteAllItems();
	char errbuf[PCAP_ERRBUF_SIZE];
	CString dev, mac, ip, net, sub, c;
	GetDlgItemText(IDC_EDIT1, dev);
	GetDlgItemText(IDC_EDIT2, ip);
	GetDlgItemText(IDC_EDIT3, mac);
	GetDlgItemText(IDC_EDIT4, net);
	GetDlgItemText(IDC_EDIT5, sub);

	if (net.IsEmpty() && sub.IsEmpty()) {
		AfxMessageBox("select subnet!");
		return;
	}

	std::string net1 = std::string(CT2CA(net));
	std::string sub1 = std::string(CT2CA(sub));

	uint32_t net_ = IPToUInt(net1) + 1;
	uint32_t sub_ = IPToUInt(sub1);
	
	if (dev.IsEmpty()) {
		AfxMessageBox(("Choose the interface!"));
		return;
	}

	pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == nullptr) {
		AfxMessageBox(("couldn't open device %s(%s)", dev, errbuf));
		return;
	}

	int index = m_iplist.GetItemCount();
	EthArpPacket request_packet;

	for (auto i = net_; i < sub_; i++) {
		int count = 0;
		request_packet.eth_.dmac_ = Mac("FF:FF:FF:FF:FF:FF");
		request_packet.eth_.smac_ = Mac((string)mac);
		request_packet.eth_.type_ = htons(EthHdr::Arp);

		request_packet.arp_.hrd_ = htons(ArpHdr::ETHER);
		request_packet.arp_.pro_ = htons(EthHdr::Ip4);
		request_packet.arp_.hln_ = Mac::SIZE;
		request_packet.arp_.pln_ = Ip::SIZE;
		request_packet.arp_.op_ = htons(ArpHdr::Request);

		request_packet.arp_.smac_ = Mac((string)mac);
		request_packet.arp_.sip_ = htonl(Ip((string)ip));
		request_packet.arp_.tmac_ = Mac("00:00:00:00:00:00");

		std::string s = ipToString(i);
		request_packet.arp_.tip_ = htonl(Ip(s));
		CString c(s.c_str());
		m_iplist.InsertItem(index, c);
		int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(&request_packet), sizeof(EthArpPacket));
		if (res != 0)
		{
			AfxMessageBox(("pcap_sendpacket return %d error %s", res, pcap_geterr(handle)));
			return;
		}

		Mac target;

		for (int i = 0; i < 20; i++) {
			struct pcap_pkthdr* header;
			const u_char* reply_packet;
			int res1 = pcap_next_ex(handle, &header, &reply_packet);
			if (res1 == 0) continue;
			if (res1 == -1 || res1 == -2) 
			{
				printf("pcap_next_ex return %d(%s)\n", res1, pcap_geterr(handle));
				break;
			}

			struct EthArpPacket* etharp = (struct EthArpPacket*)reply_packet;
			if (etharp->eth_.type_ != htons(EthHdr::Arp)) continue;
			if (etharp->arp_.op_ != htons(ArpHdr::Reply)) continue;
			if (etharp->arp_.sip_ != request_packet.arp_.tip_) continue;
			target = etharp->arp_.smac_;
			count++;
			break;
		}

		if (count == 0) {
			m_iplist.SetItemText(index, 1, "X");
			m_iplist.SetItemText(index, 2, "X");
		}
		else 
		{
			m_iplist.SetItemText(index, 2, "O");
			m_iplist.SetItemText(index, 1, "O");
		}
	}
	pcap_close(handle);
	MessageBox("arp finsihed");
	return;
}

//selecte subnet
void CNetmanDlg::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sub, sIp;
	GetDlgItemText(IDC_EDIT2, sIp);
	GetDlgItemText(IDC_EDIT6, sub);

	if (sIp.IsEmpty() || sub.IsEmpty()) {
		AfxMessageBox("Selected intrerface");
		return;
	}

	CString data;
	m_combo.GetLBText(m_combo.GetCurSel(), data);
	int m = atoi(data);

	switch (m) {
		case 16:
		{
			CString a = "255.255.0.0";
			string sIp1 = string(CT2CA(sIp));
			string sub1 = string(CT2CA(a));

			uint32_t sIp_ = IPToUInt(sIp1);
			uint32_t sub_ = IPToUInt(sub1);

			uint32_t net_lower = (sIp_ & sub_);
			uint32_t net_upper = (net_lower | (~sub_));

			CString net_lower_ = ipToString(net_lower);
			CString net_upper_ = ipToString(net_upper);
			SetDlgItemText(IDC_EDIT4, net_lower_);
			SetDlgItemText(IDC_EDIT5, net_upper_);
			break;
		}

		case 24:
		{
			string sIp1 = string(CT2CA(sIp));
			string sub1 = string(CT2CA(sub));

			uint32_t sIp_ = IPToUInt(sIp1);
			uint32_t sub_ = IPToUInt(sub1);

			uint32_t net_lower = (sIp_ & sub_);
			uint32_t net_upper = (net_lower | (~sub_));

			CString net_lower_ = ipToString(net_lower);
			CString net_upper_ = ipToString(net_upper);
			SetDlgItemText(IDC_EDIT4, net_lower_);
			SetDlgItemText(IDC_EDIT5, net_upper_);
			break;
		}

		case 32:
		{
			CString a = "255.255.255.255";
			string sIp1 = string(CT2CA(sIp));
			string sub1 = string(CT2CA(a));

			uint32_t sIp_ = IPToUInt(sIp1);
			uint32_t sub_ = IPToUInt(sub1);

			uint32_t net_lower = (sIp_ & sub_);
			uint32_t net_upper = (net_lower | (~sub_));

			CString net_lower_ = ipToString(net_lower);
			CString net_upper_ = ipToString(net_upper);
			SetDlgItemText(IDC_EDIT4, net_lower_);
			SetDlgItemText(IDC_EDIT5, net_upper_);
			break;
		}
	}
}

//SCAN
void CNetmanDlg::OnBnClickedButton7()
{
	m_iplist.DeleteAllItems();
	char errbuf[PCAP_ERRBUF_SIZE];
	CString dev, mac, ip, net, sub, com;
	GetDlgItemText(IDC_EDIT1, dev);
	GetDlgItemText(IDC_EDIT2, ip);
	GetDlgItemText(IDC_EDIT3, mac);
	GetDlgItemText(IDC_EDIT4, net);
	GetDlgItemText(IDC_EDIT5, sub);
	GetDlgItemText(IDC_COMBO1, com);

	if (net.IsEmpty() && sub.IsEmpty()) {
		AfxMessageBox("select subnet!");
		return;
	}

	string net1 = string(CT2CA(net));
	string sub1 = string(CT2CA(sub));

	uint32_t net_ = IPToUInt(net1) + 1;
	uint32_t sub_ = IPToUInt(sub1);

	if (dev.IsEmpty()) {
		AfxMessageBox(("Choose the interface!"));
		return;
	}

	pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == nullptr) {
		AfxMessageBox(("couldn't open device %s(%s)", dev, errbuf));
		return;
	}

	int index = m_iplist.GetItemCount();
	EthArpPacket request_packet;

	for (auto i = net_; i < sub_; i++) {
		int count = 0;
		request_packet.eth_.dmac_ = Mac("FF:FF:FF:FF:FF:FF");
		request_packet.eth_.smac_ = Mac((string)mac);
		request_packet.eth_.type_ = htons(EthHdr::Arp);

		request_packet.arp_.hrd_ = htons(ArpHdr::ETHER);
		request_packet.arp_.pro_ = htons(EthHdr::Ip4);
		request_packet.arp_.hln_ = Mac::SIZE;
		request_packet.arp_.pln_ = Ip::SIZE;
		request_packet.arp_.op_ = htons(ArpHdr::Request);

		request_packet.arp_.smac_ = Mac((string)mac);
		request_packet.arp_.sip_ = htonl(Ip((string)ip));
		request_packet.arp_.tmac_ = Mac("00:00:00:00:00:00");

		std::string s = ipToString(i);
		request_packet.arp_.tip_ = htonl(Ip(s));
		CString c(s.c_str());
		m_iplist.InsertItem(index, c);
		int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(&request_packet), sizeof(EthArpPacket));
		if (res != 0)
		{
			AfxMessageBox(("pcap_sendpacket return %d error %s", res, pcap_geterr(handle)));
			return;
		}

		Mac target;

		for (int i = 0; i < 30; i++) {
			struct pcap_pkthdr* header;
			const u_char* reply_packet;
			int res1 = pcap_next_ex(handle, &header, &reply_packet);
			if (res1 == 0) continue;
			if (res1 == -1 || res1 == -2)
			{
				printf("pcap_next_ex return %d(%s)\n", res1, pcap_geterr(handle));
				break;
			}

			struct EthArpPacket* etharp = (struct EthArpPacket*)reply_packet;
			if (etharp->eth_.type_ != htons(EthHdr::Arp)) continue;
			if (etharp->arp_.op_ != htons(ArpHdr::Reply)) continue;
			if (etharp->arp_.sip_ != request_packet.arp_.tip_) continue;
			target = etharp->arp_.smac_;
			count++;
			break;
		}

		if (count == 0) {
			m_iplist.SetItemText(index, 1, "X");
			m_iplist.SetItemText(index, 2, "X");
		}
		else
		{
			m_iplist.SetItemText(index, 2, "O");
			int count1 = 0;
			int nsize = sizeof(ethernet_header) + sizeof(ip_header) + sizeof(icmp_header);
			char* data = (char*)malloc(nsize);

			ethernet_header* eth = (ethernet_header*)data;
			eth->dmac_ = target;
			eth->smac_ = Mac((std::string)mac);
			eth->type_ = htons(EthHdr::Ip4);

			ip_header* iph = (ip_header*)(data + 14);
			iph->ip_hl = 0x45;
			iph->ip_tos = 0;
			iph->ip_len = ntohs(nsize);
			iph->ip_id = 0;
			iph->ip_off = 0;
			iph->ip_ttl = 128;
			iph->ip_pro = 1;
			iph->ip_chk = 0;
			iph->sip = htonl(Ip((std::string)ip));
			iph->dip = htonl(Ip(s));
			iph->ip_chk = checksum((USHORT*)iph, sizeof(ip_header));

			icmp_header* icmph = (icmp_header*)(data + 34);
			icmph->type = 8;
			icmph->code = 0;
			icmph->checksum = 0;
			icmph->id = htons(1);
			icmph->seq = htons(1);
			icmph->checksum = checksum((USHORT*)icmph, sizeof(icmp_header));


			int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(data), nsize);
			if (res != 0) {
				AfxMessageBox(("pcap_sendpacket return %d error %s", res, pcap_geterr(handle)));
				return;
			}

			for (int i = 0; i < 30; i++) {
				struct pcap_pkthdr* header;
				const u_char* icmp_reply;
				int res = pcap_next_ex(handle, &header, &icmp_reply);
				if (res == 0) continue;
				if (res == -1 || res == -2) {
					AfxMessageBox(("pcap_next_ex return %d(%s)", res, pcap_geterr(handle)));
					return;
				}

				struct ping_* icmpReply = (struct ping_*)icmp_reply;
				if (icmpReply->iph_.sip == iph->dip && icmpReply->icmp_.type == 0)
				{
					count1++;
					break;
				}
			}

			if (count1 == 0) {
				m_iplist.SetItemText(index, 1, "X");
			}
			else {
				m_iplist.SetItemText(index, 1, "O");
			}
		}
	}
	pcap_close(handle);
	MessageBox("arp finsihed");
	return;
}