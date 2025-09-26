#include  <iostream>

int main()
{
    constexpr int apple_count{15};
    constexpr int orange_count{5};

    constexpr int total_fruit_count{apple_count + orange_count};

    std::cout << "The value of apple_count is " << apple_count << std::endl;
    std::cout << "The value of orange_count is " << orange_count << std::endl;
    std::cout << "The value of total_fruit  is " << total_fruit_count << std::endl;


    signed char index{90};
    short score = {80};
    signed short value{90};
    signed short int car{700};

    unsigned int high_score{1};

    long unsigned int gold_color{0xFDCA00'00Ab};

    unsigned int octal{0564};
    std::cout << "the signed char is equivalent to Rust's u8 " << index << std::endl;
    std::cout << "the color is gold and the value is " << gold_color << std::endl;
    std::cout << "the color is gold and the value is " << octal << std::endl;
}
