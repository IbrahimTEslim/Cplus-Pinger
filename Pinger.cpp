// Pinger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>

#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

DWORD ReturnedValue;
int ttl = 1;

void main()
{
	system("cls");
	system("title Pinger");
	system("color 09");

	char Data[32] = "Icmp Request Data";

	string target;
	cout << "Enter ip target: ";
	getline(cin, target);

	int No_request;
	cout << "\nNumber of request to send: ";
	cin >> No_request;

	long long int_ip = inet_addr(target.c_str());
	if (int_ip == INADDR_NONE)
	{
		cout << "Error Converting target ip to int value" << endl;
		return;
	}

	HANDLE icmpFile = IcmpCreateFile();
	if (icmpFile == INVALID_HANDLE_VALUE)
	{
		cout << "Error Creating icmp file handle !!";
		return;
	}

	DWORD ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(Data);
	LPVOID ReaplyBuffer = new VOID* [ReplySize];
	if (ReaplyBuffer == NULL)
	{
		cout << "Unable to allocate memory !!";
		return;
	}

	for (int i = 0; i < No_request; i++)
	{
		ReturnedValue = IcmpSendEcho(icmpFile, int_ip, Data, sizeof(Data), NULL, ReaplyBuffer, ReplySize, 1000);
		if (ReturnedValue != 0)
		{
			PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReaplyBuffer;
			pEchoReply->Options.Ttl = ttl;
			struct in_addr ReplyAddr;
			ReplyAddr.S_un.S_addr = pEchoReply->Address;

			cout << endl << endl << "Send ICMP request to: " + target << endl;
			cout << "Information from this response: " << endl;
			cout << "\t Recieved from: " << inet_ntoa(ReplyAddr) << endl;
			cout << "\t Round trip = " << (int)pEchoReply->RoundTripTime << " milliseconds" << endl;

			if (pEchoReply->Status == 0)
				cout << "\t Status: " << "Success" << endl;
			else
			{
				ttl++;
				i--;
		}
			
			cout << "\t Data Sent Size: " << (int)pEchoReply->DataSize << " Byte" << endl;
			cout << "\t Time-to-Live (ttl): " << (int)pEchoReply->Options.Ttl << endl;

			Sleep(1000);
		}
		else
		{
			cout << "\n\nICMP Failed!!\nicmpSendEcho failed!!"<<endl;
			cout << "icmpSendEcho Returned Error: " << GetLastError()<<endl<<endl;
			
		}
	}



}

