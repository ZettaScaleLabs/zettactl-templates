#include <stdio.h>
#include <string.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#define sleep(x) Sleep(x * 1000)
#else
#include <unistd.h>
#endif
#include "zenoh.h"

const char *expr = "demo/example/zenoh-c-queryable";
const char *value = "Queryable from C!";
z_keyexpr_t keyexpr;

void query_handler(const z_query_t *query, void *context) {
    z_owned_str_t keystr = z_keyexpr_to_string(z_query_keyexpr(query));
    z_bytes_t pred = z_query_parameters(query);
    z_value_t payload_value = z_query_value(query);
    if (payload_value.payload.len > 0) {
        printf(">> [Queryable ] Received Query '%s?%.*s' with value '%.*s'\n", z_loan(keystr), (int)pred.len,
               pred.start, (int)payload_value.payload.len, payload_value.payload.start);
    } else {
        printf(">> [Queryable ] Received Query '%s?%.*s'\n", z_loan(keystr), (int)pred.len, pred.start);
    }
    z_query_reply_options_t options = z_query_reply_options_default();
    options.encoding = z_encoding(Z_ENCODING_PREFIX_TEXT_PLAIN, NULL);
    z_query_reply(query, z_keyexpr((const char *)context), (const unsigned char *)value, strlen(value), &options);
    z_drop(z_move(keystr));
}

int main(int argc, char **argv) {

    z_owned_config_t config = z_config_default();

    printf("Opening session...\n");
    z_owned_session_t s = z_open(z_move(config));
    if (!z_check(s)) {
        printf("Unable to open session!\n");
        exit(-1);
    }
    keyexpr = z_keyexpr(expr);
    if (!z_check(keyexpr)) {
        printf("%s is not a valid key expression", expr);
        exit(-1);
    }

    printf("Declaring Queryable on '%s'...\n", expr);
    z_owned_closure_query_t callback = z_closure(query_handler, NULL, expr);
    z_owned_queryable_t qable = z_declare_queryable(z_loan(s), keyexpr, z_move(callback), NULL);
    if (!z_check(qable)) {
        printf("Unable to create queryable.\n");
        exit(-1);
    }

    printf("Enter 'q' to quit...\n");
    char c = 0;
    while (c != 'q') {
        c = getchar();
        if (c == -1) {
            sleep(1);
        }
    }

    z_undeclare_queryable(z_move(qable));
    z_close(z_move(s));
    return 0;
}