#include <stdio.h>

#include <iostream>

#include "zenoh.hxx"
using namespace zenoh;

void print_zid(const Id* id) {
    if (id) std::cout << id << std::endl;
}

int main(int argc, char** argv) {

    printf("Opening session...\n");
    Config config;
    auto session = std::get<Session>(open(std::move(config)));

    auto self_id = session.info_zid();
    printf("own id: ");
    print_zid(&self_id);

    printf("routers ids:\n");
    session.info_routers_zid(print_zid);

    printf("peers ids:\n");
    session.info_peers_zid(print_zid);
    return 0;
}