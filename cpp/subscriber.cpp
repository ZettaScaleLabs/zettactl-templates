#include <stdio.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#define sleep(x) Sleep(x * 1000)
#else
#include <unistd.h>
#endif
#include <iostream>

#include "zenoh.hxx"
using namespace zenoh;

const char *kind_to_str(z_sample_kind_t kind);

void data_handler(const Sample *sample) {
    if (sample) {
        std::cout << ">> [Subscriber] Received " << kind_to_str(sample->get_kind()) << " ('"
                  << sample->get_keyexpr().as_string_view() << "' : '" << sample->get_payload().as_string_view()
                  << "')\n";
    }
}

int main(int argc, char **argv) {
    
    const char *expr = "demo/example/**";
    const char *locator = nullptr;

    KeyExprView keyexpr(expr);

    printf("Opening session...\n");
    Config config;
    auto session = std::get<Session>(open(std::move(config)));

    printf("Declaring Subscriber on '%s'...\n", expr);
    auto subscriber = std::get<Subscriber>(session.declare_subscriber(keyexpr, data_handler));

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

const char *kind_to_str(z_sample_kind_t kind) {
    switch (kind) {
        case Z_SAMPLE_KIND_PUT:
            return "PUT";
        case Z_SAMPLE_KIND_DELETE:
            return "DELETE";
        default:
            return "UNKNOWN";
    }
}