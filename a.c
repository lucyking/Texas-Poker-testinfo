#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int m_socket_id = -1;
char*p, str1[] = "inquire";
char inquire_msg[50] = {'\0'};

int send_server_message()
{
	snprintf(inquire_msg, sizeof(inquire_msg) - 1, "check");
	send(m_socket_id, inquire_msg, strlen(inquire_msg) + 1, 0);
	return 0;
}
/* ŽŠÀíserverµÄÏûÏ¢ */
int on_server_message(int length, const char* buffer)
{
	printf("Recieve Data From Server(%s)\n", buffer);
	p = strstr(buffer, str1);
	if(p){
		send_server_message();
	}

	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 6)
	{
		printf("Usage: %s server_ip server_port my_ip my_port my_id\n", argv[0]);
		return -1;
	}

	/* »ñÈ¡ÊäÈë²ÎÊý */
	in_addr_t server_ip = inet_addr(argv[1]);
	in_port_t server_port = htons(atoi(argv[2]));
	in_addr_t my_ip = inet_addr(argv[3]);
	in_port_t my_port = htons(atoi(argv[4]));
	int my_id = atoi(argv[5]);

	/* ŽŽœšsocket */
	m_socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if(m_socket_id < 0)
	{
		printf("init socket failed!\n");
		return -1;
	}

	/* ÉèÖÃsocketÑ¡Ïî£¬µØÖ·ÖØžŽÊ¹ÓÃ£¬·ÀÖ¹³ÌÐò·ÇÕý³£ÍË³ö£¬ÏÂŽÎÆô¶¯Ê±bindÊ§°Ü */
	int is_reuse_addr = 1;
	setsockopt(m_socket_id, SOL_SOCKET, SO_REUSEADDR, (const char*)&is_reuse_addr, sizeof(is_reuse_addr));

	/* °ó¶šÖž¶šipºÍport£¬²»È»»á±»serverŸÜŸø */
	struct sockaddr_in my_addr;
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = my_ip;
	my_addr.sin_port = my_port;
	if(bind(m_socket_id, (struct sockaddr*)&my_addr, sizeof(my_addr)))
	{
		printf("bind failed!\n");
		return -1;
	}

	/* Á¬œÓserver */
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = server_ip;
	server_addr.sin_port = server_port;
	while(connect(m_socket_id, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		usleep(100*1000); /* sleep 100ms, È»ºóÖØÊÔ£¬±£Ö€ÎÞÂÛserverÏÈÆð»¹ÊÇºóÆð£¬¶Œ²»»áÓÐÎÊÌâ */
	}

	/* Ïòserver×¢²á */
	char reg_msg[50] = {'\0'};
	snprintf(reg_msg, sizeof(reg_msg) - 1, "reg: %d %s \n", my_id, "tt");
	send(m_socket_id, reg_msg, strlen(reg_msg) + 1, 0);

	/* œÓÊÕserverÏûÏ¢£¬œøÈëÓÎÏ· */
	while(1)
	{
		char buffer[1024] = {'\0'};
		int length = recv(m_socket_id, buffer, sizeof(buffer) - 1, 0);
		if(length > 0)
		{
			/* on_server_message·µ»Ø-1£š±ÈÈçÊÕµœgame overÏûÏ¢£©£¬ÔòÌø³öÑ­»·£¬¹Ø±Õsocket£¬°²È«ÍË³ö³ÌÐò */
			if (-1 == on_server_message(length, buffer))
			{
				break;
			}
		}
	}

	/* ¹Ø±Õsocket */
	close(m_socket_id);

	return 0;
}
