#include <stdio.h>
#include <string.h>

#include <iostream>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#define sleep(x) Sleep(x * 1000)
#else
#include <unistd.h>
#endif

#include "zenoh.hxx"
using namespace zenoh;

#ifdef ZENOHCXX_ZENOHC
const char *expr = "demo/example/zenoh-cpp-zenoh-c-queryable";
const char *value = "Queryable from C++ zenoh-c!";
#elif ZENOHCXX_ZENOHPICO
const char *expr = "demo/example/zenoh-cpp-zenoh-pico-queryable";
const char *value = "Queryable from C++ zenoh-pico!";
#else
#error "Unknown zenoh backend"
#endif

int main(int argc, char **argv) {

    
    printf("Opening session...\n");
    Config config;
    auto session = std::get<Session>(open(std::move(config)));

    KeyExprView keyexpr(expr);
    if (!keyexpr.check()) {
        printf("%s is not a valid key expression", expr);
        exit(-1);
    }

    printf("Declaring Queryable on '%s'...\n", expr);

    auto queryable = std::get<Queryable>(session.declare_queryable(keyexpr, [](const Query *query) {
        if (query) {
            auto keystr = query->get_keyexpr();
            auto pred = query->get_parameters();
            auto query_value = query->get_value();
            std::cout << ">> [Queryable ] Received Query '" << keystr.as_string_view() << "?" << pred.as_string_view()
                      << "' value = '" << query_value.as_string_view() << "'\n";
            QueryReplyOptions options;
            options.set_encoding(Encoding(Z_ENCODING_PREFIX_TEXT_PLAIN));
            query->reply(expr, value, options);
        } else {
            std::cout << "Destroying queryable\n";
        }
    }));

    printf("Enter 'q' to quit...\n");
    char c = 0;
    while (c != 'q') {
        c = getchar();
        if (c == -1) {
            sleep(1);
        }
    }

    return 0;
}