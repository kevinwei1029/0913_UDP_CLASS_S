#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <iomanip>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TCPIP.h"
using namespace std;

TCPIP* lp;
SOCKET SSock;
sockaddr_in SAddr;

#define MAX 100
//vector<sockaddr> Addr;
sockaddr Addr[MAX];
int no = 0, j = 0;

int main()
{
	cout << "UDP Server :\n";

	//  1. generate UDP object
	lp = new TCPIP();

	//  2. start UDP client
	int SPort = 6000;
	lp->Start_UDP_Server(&SSock, SPort);

	//  3. read data from client and print them
	char s1[2000], s2[2000];
	int i, Len = sizeof(sockaddr);
	while (1)
	{
		//  3.1 get data
		i = recvfrom(SSock, s1, sizeof(s1), 0, (sockaddr*)&SAddr, &Len);  //  stock here

		if (i > 0)
		{
			//s1[i] = 0;
			printf("recv(%s:%d:%d)= %s\n", inet_ntoa(SAddr.sin_addr), SAddr.sin_port, i, s1);

			for (j = 0; j < no; j++) {
				if (!memcmp(&Addr[j], &SAddr, sizeof(SAddr))) break;
			}
			if (j < no) {}
			else {
				//cout << "1st if loop\n";
				memcpy(&Addr[j], &SAddr, Len);
				no++;
			}

			sprintf_s(s2, sizeof(s2), "read");
			sendto(SSock, s2, strlen(s2), 0, (sockaddr*)&SAddr, Len);
			//  3.2 send data
			for (int k = 0; k < no; k++) {
				sendto(SSock, s1, strlen(s1), 0, (sockaddr*)&Addr[k], Len);
			}

			//  3.3 send card
			if (no >= 4) {
				vector<string> card;
				for (string i : {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"}) {
					for (string j : { "S", "H", "D", "C" }) {
						card.push_back(i + j);
					}
				}
				for (int i = 0; i < no; i++) {
					string txt = "you get card:";
					for (int j = 0; j < 13; j++) {
						int r = rand() % card.size();
						txt += card[r] + " ";
						card.erase(card.begin() + r);
					}
					sendto(SSock, txt.c_str(), txt.size(), 0, (sockaddr*)&Addr[i], Len);
				}
			}
		}
	}
}
