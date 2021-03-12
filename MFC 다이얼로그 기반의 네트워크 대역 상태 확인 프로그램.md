## MFC 다이얼로그 기반의 네트워크 대역 상태 확인 프로그램

##   Contents

* Section 1. Requirements
* Section 2. Data Design
  * 2-1. E-R Diagram
  * 2-2. Table
* Section 3. Architectural Design
  * 3-1. Data Flow Diagram
  * 3-2 Data Dictionary
  * 3-3 Mini Specifications
  * 3-4 Architectural Context Diagram
* Section 4. Interface Design (User Fuction Categories)
* Section 5. Procedural Design



### Section 1. Requirements

* MFC 다이얼로그 기반 프로젝트 사용
* 현재 PC에서 사용중인 NIC 정보 표시
* 사용 편의성 고려(리스트 컨트롤)
* NIC 선택 후 해당 NIC을 기준으로 ip 충돌 방지를 위해 send_arp
*  send_icmp
* 개별 IP 추가/삭제 가능
* IP 대역 추가 삭제 기능
* 멀티 check로 삭제가 가능
* 리스트 컨트롤을 사용하여 추가된 IP 확인이 용이
* pcap library 사용



### Section 2. Data Design

* 2-1. E-R Diagram

![image-20210312162630862](C:\Users\yeom\AppData\Roaming\Typora\typora-user-images\image-20210312162630862.png)

* 2-2 Table

  #### NIC

  | Element Name                                          | Data Type |
  | ----------------------------------------------------- | --------- |
  | m_niclist                                             | CListCtrl |
  | name                                                  | char      |
  | description                                           | char      |
  | inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr    | char      |
  | inet_ntoa(((struct sockaddr_in*)a->netmask)->sin_addr | char      |

  #### ARP

  | Element Name     | Data Type          |
  | ---------------- | ------------------ |
  | dmac_            | uint8_t -> string  |
  | smac_ (ethernet) | uint8_t -> string  |
  | type_            | uint16_t           |
  | hrd_             | uint16_t           |
  | pro_             | uint16_t           |
  | hln_             | uint8_t            |
  | pln_             | uint8_t            |
  | op_              | uint16_t           |
  | smac_ (arp)      | uint8_t -> string  |
  | sip_             | uint32_t -> string |
  | tmac_ (arp)      | uint8_t -> string  |
  | tip_             | uin32_t -> string  |

  #### ICMP

  | Element Name  | Data Type          |
  | ------------- | ------------------ |
  | dmac          | uint8_t -> string  |
  | smac          | uint8_t -> string  |
  | type          | uint16_t           |
  | ip_hl         | uint8_t            |
  | ip_tos        | uint8_t            |
  | ip_total_len  | uint16_t           |
  | ip_id         | uint16_t           |
  | ip_off        | uint16_t           |
  | ip_ttl        | uint8_t            |
  | ip_protocol   | uint16_t           |
  | sip           | uint32_t -> string |
  | dip           | uint32_t -> string |
  | icmp_type     | uint8_t            |
  | icmp_code     | uint8_t            |
  | icmp_checksum | uint16_t           |
  | icmp_id       | uint16_t           |
  | icmp_seq      | uint16_t           |

  #### IP allocation

  | Element Name | Data Type      |
  | ------------ | -------------- |
  | m_iplist     | CListCtrl      |
  | m_ip         | CIPAddressCtrl |
  | m_sub        | CIPAddressCtrl |

  

### Section 3. Architectural Design







### Section 4. Interface Design (User Fuction Categories)

![image-20210312144205106](C:\Users\yeom\AppData\Roaming\Typora\typora-user-images\image-20210312144205106.png)

1. NIC 정보가 출력되면 원하는 인터페이스 선택 후 select 버튼을 눌러준다.
2. ARP 버튼을 누름으로서 IP 충돌 방지를 해준다.
3. PING 버튼을 누름으로서 네트워크 상태 확인한다.
4. 2,3번의 과정을 거친 IP를 IP 대역과 같이 IP List 안에 넣어준다.
5. DELETE 버튼은 원하는 IP를 삭제할 수 있다. 물론 다중선택도 가능하다
6. 편의를 위해 모든 Ip list를 삭제하는 버튼을 생성했다.



