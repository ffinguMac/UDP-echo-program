#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char* argv[]) {
    int s_socket; // 서버 소켓
    char msg[1024]; // 메시지 송수신을 위한 버퍼
    struct sockaddr_in serv_addr; // 서버 주소 구조체
    struct sockaddr_in clnt_addr; // 클라이언트 주소 구조체
    socklen_t clnt_addr_size; // 클라이언트 주소 구조체 크기 저장 변수

    printf("server open\n");

    // UDP 서버 소켓 생성
    s_socket = socket(PF_INET, SOCK_DGRAM, 0);

    // 서버 주소 구조체 초기화
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스에서 들어오는 연결 수락
    serv_addr.sin_port = htons(atoi(argv[1])); // 서버 포트 번호 설정
    
    // 소켓과 주소를 바인딩
    bind(s_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while(1) {
        clnt_addr_size = sizeof(clnt_addr);
        recvfrom(s_socket, msg, sizeof(msg), 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size); // 클라이언트로부터 메시지 수신
        printf("receive msg: %s", msg);
        sendto(s_socket, msg, sizeof(msg), 0, (struct sockaddr*)&clnt_addr, clnt_addr_size); // 클라이언트에게 수신한 메시지를 다시 전송
        memset(msg, 0, sizeof(msg));
    }

    close(s_socket); // 소켓 닫기

    return 0;
}
