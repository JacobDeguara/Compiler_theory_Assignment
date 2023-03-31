#include <iostream>

#include "Lexer.cpp"

#define ASSERT(x)                                                                     \
    {                                                                                 \
        if (!(x))                                                                     \
            std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; \
        else                                                                          \
            std::cout << __FUNCTION__ << " > PASS" << std::endl;                      \
    }

void test_lexer_set_up_succeeds(Lexer lexer)
{

    ASSERT(lexer.set_up_file("test.txt"));
}

void test_lexer_set_up_fails(Lexer lexer)
{

    ASSERT(!lexer.set_up_file("doesntExists.txt"));
}

void test_lexer_unit_tests()
{
    Lexer lexer = Lexer();
    test_lexer_set_up_fails(lexer);
    test_lexer_set_up_succeeds(lexer);
}

int main(void)
{

    test_lexer_unit_tests();
}