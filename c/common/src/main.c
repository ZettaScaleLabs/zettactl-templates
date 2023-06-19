#include <stdio.h>

#include "zenoh.h"

void print_zid(const z_id_t *id, void *ctx) {
    for (int i = 0; i < 16; i++) {
        printf("%02x", id->id[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    z_owned_config_t config = z_config_default();

    printf("Opening session...\n");
    z_owned_session_t s = z_open(z_move(config));
    if (!z_check(s)) {
        printf("Unable to open session!\n");
        exit(-1);
    }

    z_id_t self_id = z_info_zid(z_loan(s));
    printf("own id: ");
    print_zid(&self_id, NULL);

    printf("routers ids:\n");
    z_owned_closure_zid_t callback = z_closure(print_zid);
    z_info_routers_zid(z_loan(s), z_move(callback));

    // `callback` has been `z_move`d just above, so it's safe to reuse the variable,
    // we'll just have to make sure we `z_move` it again to avoid mem-leaks.
    printf("peers ids:\n");
    z_owned_closure_zid_t callback2 = z_closure(print_zid);
    z_info_peers_zid(z_loan(s), z_move(callback2));

    z_close(z_move(s));
}