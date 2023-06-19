#include <stdio.h>
#include <string.h>

#include <iostream>
#include <limits>
#include <sstream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#define sleep(x) Sleep(x * 1000)
#else
#include <unistd.h>
#endif

#include "zenoh.hxx"
using namespace zenoh;

#ifdef ZENOHCXX_ZENOHC
const char *default_value = "Pub from C++ zenoh-c!";
const char *default_keyexpr = "demo/example/zenoh-cpp-zenoh-c-pub";
#elif ZENOHCXX_ZENOHPICO
const char *default_value = "Pub from C++ zenoh-pico!";
const char *default_keyexpr = "demo/example/zenoh-cpp-zenoh-pico-pub";
#else
#error "Unknown zenoh backend"
#endif

int main(int argc, char **argv) {
    
    const char *keyexpr = default_keyexpr;
    const char *value = default_value;

    printf("Opening session...\n");
    Config config;
    auto session = std::get<Session>(open(std::move(config)));

    printf("Declaring Publisher on '%s'...\n", keyexpr);
    auto pub = std::get<Publisher>(session.declare_publisher(keyexpr));

    PublisherPutOptions options;
    options.set_encoding(Z_ENCODING_PREFIX_TEXT_PLAIN);
    for (int idx = 0; std::numeric_limits<int>::max(); ++idx) {
        sleep(1);
        std::ostringstream ss;
        ss << "[" << idx << "] " << value;
        auto s = ss.str();  // in C++20 use .view() instead
        std::cout << "Putting Data ('" << keyexpr << "': '" << s << "')...\n";
        pub.put(s);
    }
    return 0;
}