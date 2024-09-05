#ifndef GLOBAL_H
#define GLOBAL_H

#include "Arduino.h"
#include "LoRaWan_APP.h"
#include <mbedtls/sha256.h>
#include "rs485.h"
#include "bsp.h"

extern byte responseBuffer[];
extern esp_sleep_wakeup_cause_t wakeup_reason;
void printHex(byte *data, int length);

esp_sleep_wakeup_cause_t print_wakeup_reason();
bool is_a_string(const char *ptr, const int maxSize);
uint64_t get_chip_id();
uint32_t simpleHash(const char *str);
void print_bytes_reverse(uint8_t *data, int length);
void print_bytes(uint8_t *data, int length);
#endif