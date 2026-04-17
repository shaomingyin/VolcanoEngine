//
//
#include <chrono>
#include <iostream>
#include <cstring>

#include <argh.h>
#include <rocket.hpp>
#include <sigslot/signal.hpp>

#include <Volcano/Server/Common.h>

static uint64_t n = 0;

#define CON(a, b) CON_(a, b)
#define CON_(a, b) a##b

static void foo0(int v, const char* p) { n += v + 1 + strlen(p); }
static void foo1(int v, const char* p) { n += v + 2 + strlen(p); }
static void foo2(int v, const char* p) { n += v + 3 + strlen(p); }
static void foo3(int v, const char* p) { n += v + 4 + strlen(p); }
static void foo4(int v, const char* p) { n += v + 5 + strlen(p); }
static void foo5(int v, const char* p) { n += v + 6 + strlen(p); }
static void foo6(int v, const char* p) { n += v + 7 + strlen(p); }
static void foo7(int v, const char* p) { n += v + 8 + strlen(p); }
static void foo8(int v, const char* p) { n += v + 9 + strlen(p); }
static void foo9(int v, const char* p) { n += v + 10 + strlen(p); }

int main(int argc, char* argv[]) {
    sigslot::signal_st<int, const char*> sigslot1;
    sigslot1.connect(&foo0);

    using clock = std::chrono::high_resolution_clock;

    auto begin = clock::now();
    for (size_t i = 0; i < 10000; ++i) {
        sigslot1(i, "asdf");
    }
    auto elapsed = clock::now() - begin;

    std::cout << n << ": " << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count() << std::endl;


    rocket::signal<void(int, const char*)> rocket1;
    rocket1.connect(&foo0);

    n = 0;
    begin = clock::now();
    for (size_t i = 0; i < 10000; ++i) {
        rocket1(i, "asdf");
    }
    elapsed = clock::now() - begin;

    std::cout << n << ": " << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count() << std::endl;

    return EXIT_SUCCESS;
}
