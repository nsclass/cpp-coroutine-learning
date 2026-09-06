#include <coroutine>
#include <format>
#include <iostream>
#include <utility>
#include <string_view>

using namespace std;

struct Fib {
    struct promise_type {
        int current;
        Fib get_return_object() { LOG(); return Fib{coroutine_handle<promise_type>::from_promise(*this)}; }
        suspend_always initial_suspend() noexcept { LOG(); return {}; }
        suspend_always final_suspend() noexcept { LOG(); return {}; }
        suspend_always yield_value(int v) noexcept { LOG(); current = v; return {}; }
        void unhandled_exception() { LOG(); throw; }
    };

    coroutine_handle<promise_type> h;
    ~Fib() { LOG(); h.destroy(); }

    int next() { LOG(); h.resume(); return h.promise().current; }
};

Fib generate() {
    LOG();
    int prev = 1;
    int next = 2;
    while (true) {
        co_yield prev;
        prev = exchange(next, prev + next);
    }
}

int main() {
    LOG();
    auto fib = generate();
    for (int i = 0; i < 1; ++i)
        cout << format("{} ", fib.next());
    cout << '\n';
}