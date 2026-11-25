#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Steam pairs every device with a key exchange, so the secret key is per host.
 * Keys are kept in the app's preference directory, one file per host client ID.
 */

#define HOST_KEY_SIZE 32

bool host_keys_load(uint64_t client_id, uint8_t key[HOST_KEY_SIZE]);

bool host_keys_save(uint64_t client_id, const uint8_t key[HOST_KEY_SIZE]);
