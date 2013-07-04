#include "hikari/client/Main.hpp"
#include "hikari/client/Client.hpp"

int main(int argc, char** argv) {
    return hikari::Client(argc, argv).run();
}