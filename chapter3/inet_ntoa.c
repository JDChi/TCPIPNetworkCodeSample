#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
	struct sockaddr_in addr1, addr2;
	char *str_ptr;
	char str_arr[20];

	addr1.sin_addr.s_addr = htonl(0x1020304);
	addr2.sin_addr.s_addr = htonl(0x1010101);

	str_ptr = inet_ntoa(addr1.sin_addr);
	strcpy(str_arr , str_ptr);
	printf("Dotted-Decimal notation1 : %s \n", str_ptr);

    // 再次调用，覆盖 str_ptr 的内容
	inet_ntoa(addr2.sin_addr);

	printf("Dotted-Decimal notation2 : %s \n", str_ptr);

	// 打印str_arr，与原先的str_ptr一样，说明已经复制出来
	printf("Dotted-Decimal notation3 : %s \n", str_arr);
	return 0;
}