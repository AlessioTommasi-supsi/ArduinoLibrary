#ifndef DNS_SERVER_ESP_IDF_H
#define DNS_SERVER_ESP_IDF_H

#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace smarthome {

class DnsServerEspIdf {
private:
    TaskHandle_t task_handle_{nullptr};
    int sock_{-1};
    volatile bool running_{false};
    uint32_t resolved_ip_{0};

    static void task_worker(void *param);

public:
    DnsServerEspIdf();
    ~DnsServerEspIdf();

    bool start(uint16_t port = 53, const char *ip_str = "192.168.4.1");
    void stop();
};

} // namespace smarthome

#endif // DNS_SERVER_ESP_IDF_H
