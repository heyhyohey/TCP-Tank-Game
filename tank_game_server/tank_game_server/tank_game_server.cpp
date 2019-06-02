#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <winsock2.h>

#define BUF_SIZE 20

// 버퍼의 크기
#define	BUF_SIZE 20

// 맵의 크기 정의
#define MAP_LINES 25
#define MAP_COLS 111

// 유저의 수 정의
#define USERS 2

// 방향 상수 정의
#define EAST 0
#define WEST 1
#define SOUTH 2
#define NORTH 3

void ErrorHandling(const char *message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr, clientaddr;
	TIMEVAL timeout;
	fd_set reads, cpyReads;

	int adrSz;
	int strLen, fdNum, i;
	int buf[BUF_SIZE];
	int result[BUF_SIZE];
	int addrlen;

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
		ErrorHandling("WSAStartup() error!"); 

	hServSock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family=AF_INET;
	servAdr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAdr.sin_port=htons(atoi(argv[1]));
	
	if(bind(hServSock, (SOCKADDR*) &servAdr, sizeof(servAdr))==SOCKET_ERROR)
		ErrorHandling("bind() error");
	if(listen(hServSock, 5)==SOCKET_ERROR)
		ErrorHandling("listen() error");

	FD_ZERO(&reads);
	FD_SET(hServSock, &reads);

	while(1)
	{
		cpyReads=reads;
		timeout.tv_sec=5;
		timeout.tv_usec=5000;

		if( (fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR )
			break;
		
		if( fdNum == 0 )
			continue;

		for(i=0; i<reads.fd_count; i++)
		{
			if(FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				if(reads.fd_array[i]==hServSock)     // connection request!
				{
					adrSz=sizeof(clntAdr);
					hClntSock=
						accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
					FD_SET(hClntSock, &reads);
					printf("connected client: Port:%d, IP:%s \n", 
						clntAdr.sin_port, inet_ntoa(clntAdr.sin_addr)	
					);
				}
				else    // read message!
				{
					int key;

					// 키값을 받음
					strLen=recv(reads.fd_array[i], (char*)buf, sizeof(buf), 0);

					if (strLen <= 0)    // close request!
					{
						FD_CLR(reads.fd_array[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						printf("closed client: %d, StrLen:%d \n",
							cpyReads.fd_array[i], strLen);
					}
					else
					{
						// 클라이언트 정보 얻기						
						addrlen = sizeof(clientaddr);
						getpeername(reads.fd_array[i], (SOCKADDR *)&clientaddr, &addrlen);

						printf("(Port:%d, IP:%s),Msg : %d %d %d %d %d %d %d \n",
							clientaddr.sin_port, inet_ntoa(clientaddr.sin_addr)
							, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);

						// key값 초기화
						key = buf[6];

						// 플레이어1 정보 저장
						result[0] = buf[0];
						result[1] = buf[1];
						result[2] = buf[2];
						// 플레이어2 정보 저장
						result[3] = buf[3];
						result[4] = buf[4];
						result[5] = buf[5];
						// 레이저 정보 저장
						result[6] = 0;
						result[7] = 0;
						result[8] = 0;
						result[9] = 0;
						switch (key) {
						case 75: // 왼쪽 방향키 눌렀을 때
							if (buf[0] <= 6) {	// 왼쪽 필드 밖으로 나가는지 확인
								result[2] = WEST;
								if (buf[0] <= 4) result[2] = buf[2];
								break;
							}
							// 플레이어1의 정보 변경
							result[0] = buf[0] - 2;
							result[2] = WEST;
							break;
						case 77: // 오른쪽 방향키 눌렀을 때
							if (buf[0] >= MAP_COLS - 7) {	// 오른쪽 필드 밖으로 나가는지 확인
								result[2] = EAST;
								if (buf[0] >= MAP_COLS - 5) result[2] = buf[2];
								break;
							}
							// 플레이어1의 정보 변경
							result[0] = buf[0] + 2;
							result[2] = EAST;
							break;
						case 80: // 아래쪽 방향키 눌렀을 때
							if (buf[1] >= MAP_LINES - 4) {	// 아래쪽 필드 밖으로 나가는지 확인
								result[2] = SOUTH;
								if (buf[1] >= MAP_LINES - 3) result[2] = buf[2];
								break;
							}
							result[1] = buf[1] + 1;
							result[2] = SOUTH;
							break;
						case 72: // 위쪽 방향키 눌렀을 때
							if (buf[1] <= 3) {	// 위쪽 필드 밖으로 나가는지 확인
								result[2] = NORTH;
								if (buf[1] <= 2) result[2] = buf[2];
								break;
							}
							result[1] = buf[1] - 1;
							result[2] = NORTH;
							break;
						case 32: // 스페이스바 눌렀을 때(발사)
							// 레이저 정보 저장
							result[6] = 1;	// 레이져 발사여부
							result[7] = buf[0];	// x좌표저장
							result[8] = buf[1]; // y좌표저장
							result[9] = buf[2];	// 방형저장
							puts("1p 발사\n");
							break;
						case 97:	// a눌렀을 때(발사)
							// 레이저 정보 저장
							result[6] = 2;	// 레이져 발사여부
							result[7] = buf[3];	// x좌표저장
							result[8] = buf[4]; // y좌표저장
							result[9] = buf[5];	// 방형저장
							puts("2p 발사\n");
							break;
						case 102:	// f눌렀을 때(왼쪽)
							if (buf[3] <= 6) {	// 왼쪽 필드 밖으로 나가는지 확인
								result[5] = WEST;
								if (buf[3] <= 4) result[5] = buf[5];
								break;
							}
							// 플레이어1의 정보 변경
							result[3] = buf[3] - 2;
							result[5] = WEST;
							break;
						case 104:	// h눌렀을 때(오른쪽)
							if (buf[3] >= MAP_COLS - 7) {	// 오른쪽 필드 밖으로 나가는지 확인
								result[5] = EAST;
								if (buf[3] >= MAP_COLS - 5) result[5] = buf[5];
								break;
							}
							// 플레이어1의 정보 변경
							result[3] = buf[3] + 2;
							result[5] = EAST;
							break;
						case 103:	// g눌렀을 때(아래쪽)
							if (buf[4] >= MAP_LINES - 4) {	// 아래쪽 필드 밖으로 나가는지 확인
								result[5] = SOUTH;
								if (buf[4] >= MAP_LINES - 3) result[5] = buf[5];
								break;
							}
							result[4] = buf[4] + 1;
							result[5] = SOUTH;
							break;
						case 116:	// t눌렀을 때(위쪽)
							if (buf[4] <= 3) {	// 위쪽 필드 밖으로 나가는지 확인
								result[5] = NORTH;
								if (buf[4] <= 2) result[5] = buf[5];
								break;
							}
							result[4] = buf[4] - 1;
							result[5] = NORTH;
							break;
						}
						send(reads.fd_array[i], (char*)result, sizeof(result), 0);
					}
				}
			}
		}
	}
	closesocket(hServSock);
	WSACleanup();
	_getch();
	return 0;
}

void ErrorHandling(const char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}