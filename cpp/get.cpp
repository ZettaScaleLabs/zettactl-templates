#include <stdio.h>
#include <string.h>

#include <condition_variable>
#include <iostream>
#include <mutex>

#include "zenoh.hxx"
using namespace zenoh;

int main(int argc, char **argv) {
    const char *expr = "demo/example/**";
    const char *value = nullptr;
    const char *locator = nullptr;

    KeyExprView keyexpr(expr);
    if (!keyexpr.check()) {
        printf("%s is not a valid key expression", expr);
        exit(-1);
    }

    printf("Opening session...\n");
    Config config;
    auto session = std::get<Session>(open(std::move(config)));

    std::cout << "Sending Query '" << expr << "'...\n";
    GetOptions opts;
    opts.set_target(Z_QUERY_TARGET_ALL);
    opts.set_value(value);

    std::mutex m;
    std::condition_variable done_signal;
    bool done = false;

    session.get(
        keyexpr, "",
        [&m, &done, &done_signal](Reply reply) {
            if (reply.check()) {
                auto result = reply.get();
                if (auto sample = std::get_if<Sample>(&result)) {
                    std::cout << "Received ('" << sample->get_keyexpr().as_string_view() << "' : '"
                              << sample->get_payload().as_string_view() << "')\n";
                } else if (auto error = std::get_if<ErrorMessage>(&result)) {
                    std::cout << "Received an error :" << error->as_string_view() << "\n";
                }
            } else {
                // No more replies
                std::lock_guard lock(m);
                done = true;
                done_signal.notify_all();
            }
        },
        opts);

    std::unique_lock lock(m);
    done_signal.wait(lock, [&done] { return done; });

    return 0;
}