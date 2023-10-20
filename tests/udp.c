#include "../udp.h"

int main() {
  udp_init();
  UDPSOCK *s = udp_open("127.0.0.1", 12345);
  if (s) {
    udp_send(s, (BYTE *)"Hello, world!", 14);
    size_t len = 0;
    BYTE *data = NULL;
    udp_recv(s, &data, &len);
  }
  udp_cleanup();
}