#ifndef TEST_HPP
#define TEST_HPP

#include <csignal>
#include <iostream>
#include <cstdlib>
#include "color.hpp"

namespace test
{

    struct Base
    {

        std::string *base_ptr;
        static int base_leak;

        Base()
        {
            base_ptr = new std::string("Base Default C/tor");
            base_leak++;
        }

        Base(const Base &other)
        {
            (void)other;
            base_ptr = new std::string("Base Copy C/tor");
            base_leak++;
        }

        virtual ~Base()
        {
            // std::cout << "Base D/tor [" << *base_ptr << "]" << std::endl;
            // std::cout << base_leak << '\n';
            base_leak--;
            // std::cout << base_leak << '\n';
            delete base_ptr;
        }

        static void announce() { std::cout << "Hello World" << std::endl; }
    };

    struct Derived : public Base
    {

        std::string *derived_ptr;
        static int derived_leak;

        Derived() : Base()
        {
            derived_ptr = new std::string("Derived Default C/tor");
            derived_leak++;
        }

        Derived(const Derived &other) : Base(other)
        {
            (void)other;
            derived_ptr = new std::string("Derived Copy C/tor");
            derived_leak++;
        }

        ~Derived()
        {
            // std::cout << "Derived D/tor [" << *derived_ptr << "]" << std::endl;
            // std::cout << derived_leak << '\n';
            derived_leak--;
            // std::cout << derived_leak << '\n';
            delete derived_ptr;
        }
    };

    void sigsegv(int signal)
    {
        (void)signal;
        std::cout << _BLUE_ << "SIGSEGV" << _RESET_ << std::endl;
        exit(EXIT_SUCCESS);
    }
}

#endif
