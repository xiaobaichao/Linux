#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "ipc.h"

int main() {
	int fd;
	struct sockaddr_un addr;
	int ret;
	char buff[8192];
	struct sockaddr_un from;
	int ok = 1;
	int len;

	if ((fd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		ok = 0;
	}

	if (ok) {
		memset(&addr, 0, sizeof(addr));
		addr.sun_family = AF_UNIX;
		strcpy(addr.sun_path, CLIENT_SOCK_FILE);
		unlink(CLIENT_SOCK_FILE);
		if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("bind");
			ok = 0;
		}
	}

	if (ok) {
		memset(&addr, 0, sizeof(addr));
		addr.sun_family = AF_UNIX;
		strcpy(addr.sun_path, SERVER_SOCK_FILE);
		if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
			perror("connect");
			ok = 0;
		}
	}

	for(int i=0;i<100;i++) {
		if (ok) {
			strcpy (buff, "1,0.909,2.261,0.274,2.605,0.472,-0.429,-0.983,1.000,-0.095,-1.219,-0.369,-0.312,-0.840,1.281,-0.618,-0.532,-0.132,0.443,0.028,2.201,0.044,1.671,0.660,-0.114,0.574,0.276,0.680,-0.670,2.1,3.4");
			if (send(fd, buff, strlen(buff)+1, 0) == -1) {
				perror("send");
				ok = 0;
			}
			printf ("sent to server%s\n",buff);
		}

		if (ok) {
			if ((len = recv(fd, buff, 8192, 0)) < 0) {
				perror("recv");
				ok = 0;
			}
			printf ("receive from server %s\n", buff);
		}
	}

	if (fd >= 0) {
		close(fd);
	}

	unlink (CLIENT_SOCK_FILE);

	return 0;
}
