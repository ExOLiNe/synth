#include <iostream>
#include "../Source/other/polymorphic_readonly_array.h"

struct Base {
    virtual void func() = 0;
    virtual ~Base() {};
};

struct A : Base {
    void func() override {

    }
    ~A() override = default;
};

struct B : Base {
    void func() override {

    }
    ~B() override = default;
};

void print(polymorphic_readonly_array<Base, A, B> vec, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        vec[i]->func();
    }
}

size_t total = 10000;

std::vector<Base*> getVector() {
    std::vector<Base*> v;
    v.reserve(total);
    for (size_t i = 0; i < total; ++i) {
        Base* item;
        if (i % 2 == 0) {
            item = new B();
        } else {
            item = new A();
        }
        v.push_back(item);
    }
    return v;
}

void testCopyConstructor() {
    auto v = getVector();

    polymorphic_readonly_array<Base, A, B> polyV(v);

    print(polyV, total);
}

void testAssignmentOperator() {
    auto v = getVector();

    polymorphic_readonly_array<Base, A, B> polyV1(v);

    std::vector<Base*> v2;
    v2.push_back(new A());
    v2.push_back(new B());

    polymorphic_readonly_array<Base, A, B> polyV2(v2);

    polyV2 = polyV1;

    for (size_t i = 0; i < total; ++i) {
        polyV1[i]->func();
        polyV2[i]->func();
    }
}

int main() {
    testCopyConstructor();

    testAssignmentOperator();
}