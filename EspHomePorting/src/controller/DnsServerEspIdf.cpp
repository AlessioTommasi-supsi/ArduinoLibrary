#include "DnsServerEspIdf.h"
#include <string.h>
#include "esp_log.h"

namespace smarthome {

static const char *TAG = "DnsServerEspIdf";

DnsServerEspIdf::DnsServerEspIdf() {}

DnsServerEspIdf::~DnsServerEspIdf() {
    stop();
}

bool DnsServerEspIdf::start(uint16_t port, const char *ip_str) {
    if (running_) return true;

    resolved_ip_ = inet_addr(ip_str);
    running_ = true;

    xTaskCreate(task_worker, "dns_captive_task", 4096, this, 3, &task_handle_);
    return true;
}

void DnsServerEspIdf::stop() {
    running_ = false;
    if (sock_ >= 0) {
        close(sock_);
        sock_ = -1;
    }
    if (task_handle_ != nullptr) {
        vTaskDelete(task_handle_);
        task_handle_ = nullptr;
    }
}

void DnsServerEspIdf::task_worker(void *param) {
    auto *self = static_cast<DnsServerEspIdf*>(param);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(53);

    self->sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (self->sock_ < 0) {
        ESP_LOGE(TAG, "Impossibile creare socket UDP DNS");
        vTaskDelete(nullptr);
        return;
    }

    if (bind(self->sock_, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(TAG, "Impossibile associare porta 53 per DNS");
        close(self->sock_);
        self->sock_ = -1;
        vTaskDelete(nullptr);
        return;
    }

    ESP_LOGI(TAG, "DNS Captive Server avviato su porta 53");

    uint8_t buffer[512];
    while (self->running_) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int len = recvfrom(self->sock_, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);

        if (len < 12) {
            continue; // Pacchetto troppo corto per contenere un header DNS
        }

        // Verifica tipo query (QR bit = 0, OPCODE = 0)
        uint16_t flags = (buffer[2] << 8) | buffer[3];
        if ((flags & 0x7800) != 0) {
            continue;
        }

        // Troviamo la fine del record Question
        int idx = 12;
        while (idx < len && buffer[idx] != 0) {
            idx += (buffer[idx] + 1);
        }
        idx += 1; // Salta il terminatore 0x00
        idx += 4; // Salta QTYPE (2 bytes) e QCLASS (2 bytes)

        if (idx > len || idx + 16 > (int)sizeof(buffer)) {
            continue; // Formato non valido
        }

        // Prepara risposta DNS:
        // Setta flags: QR=1 (risposta), AA=1 (autorevole), RA=1, No error (0x8180)
        buffer[2] = 0x81;
        buffer[3] = 0x80;

        // Imposta ANCOUNT = 1 (1 risposta inclusa)
        buffer[6] = 0x00;
        buffer[7] = 0x01;

        // Costruisci Answer Record alla fine della question
        buffer[idx++] = 0xC0; // Puntatore a offset 12 (nome della query)
        buffer[idx++] = 0x0C;

        buffer[idx++] = 0x00; // Type A (IPv4)
        buffer[idx++] = 0x01;

        buffer[idx++] = 0x00; // Class IN
        buffer[idx++] = 0x01;

        buffer[idx++] = 0x00; // TTL: 60 secondi
        buffer[idx++] = 0x00;
        buffer[idx++] = 0x00;
        buffer[idx++] = 0x3C;

        buffer[idx++] = 0x00; // Lunghezza dati (4 bytes)
        buffer[idx++] = 0x04;

        // Indirizzo IP di risposta (192.168.4.1 in byte order)
        memcpy(&buffer[idx], &self->resolved_ip_, 4);
        idx += 4;

        sendto(self->sock_, buffer, idx, 0, (struct sockaddr *)&client_addr, client_len);
    }

    if (self->sock_ >= 0) {
        close(self->sock_);
        self->sock_ = -1;
    }
    vTaskDelete(nullptr);
}

} // namespace smarthome
