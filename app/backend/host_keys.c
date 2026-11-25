#include "host_keys.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <SDL.h>

#include "logging.h"

static bool host_key_path(uint64_t client_id, char *path, size_t path_len) {
    char *pref_path = SDL_GetPrefPath("mariotaku", "ihsplay");
    if (pref_path == NULL) {
        commons_log_error("HostKeys", "Can't get preference path: %s", SDL_GetError());
        return false;
    }
    // SDL for webOS returns the app directory without the trailing separator other platforms have.
    size_t pref_len = strlen(pref_path);
    const char *sep = pref_len > 0 && pref_path[pref_len - 1] == '/' ? "" : "/";
    int len = snprintf(path, path_len, "%s%shostkey_%016llx.bin", pref_path, sep, (unsigned long long) client_id);
    SDL_free(pref_path);
    return len > 0 && (size_t) len < path_len;
}

bool host_keys_load(uint64_t client_id, uint8_t key[HOST_KEY_SIZE]) {
    char path[1024];
    if (!host_key_path(client_id, path, sizeof(path))) {
        return false;
    }
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return false;
    }
    bool ok = fread(key, 1, HOST_KEY_SIZE, f) == HOST_KEY_SIZE;
    fclose(f);
    if (!ok) {
        commons_log_warn("HostKeys", "Ignoring malformed key file %s", path);
    }
    return ok;
}

bool host_keys_save(uint64_t client_id, const uint8_t key[HOST_KEY_SIZE]) {
    char path[1024];
    if (!host_key_path(client_id, path, sizeof(path))) {
        return false;
    }
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        commons_log_error("HostKeys", "Can't write %s", path);
        return false;
    }
    chmod(path, 0600);
    bool ok = fwrite(key, 1, HOST_KEY_SIZE, f) == HOST_KEY_SIZE;
    ok = fclose(f) == 0 && ok;
    if (ok) {
        commons_log_warn("HostKeys", "Saved pairing key to %s", path);
    } else {
        commons_log_error("HostKeys", "Failed to write %s", path);
    }
    return ok;
}
