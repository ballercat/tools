#include "../toolkit.h"
#include <iostream>
#include <cstdint>

bool tuple_tests()
{
    auto value_test = [](auto t) { //C++14 is crazy
        auto v0 = 12.2;
        auto v1 = 42u;
        auto v2 = "big";

        std::cout << std::boolalpha;
        auto result =  (  (types::get_from<0>(t) == v0) &&
                          (types::get_from<1>(t) == v1) &&
                          (types::get_from<2>(t) == v2) );

        std::cout << "\tValue test: " << result << std::endl;
    };

    types::tuple<double, uint32_t, const char*> t1{ 12.2, 42, "big" };

    std::cout << "Tuple tests.\n";
    std::cout << "Tuple size: " << types::tuple_size(t1) << std::endl;

    auto t2 = t1; //...
    value_test(t1);
    types::get_from<1>(t1) = 103;
    t1 = t2; //What happens here? What happens to the original data in t1?
    value_test(t1);
    value_test(t2);

    types::tuple<double, uint32_t, const char*> t3{ 1.1, 0, "cat" };

    //std::cout << (t1 == t3) << std::endl;
    return true;
}

bool item_tests()
{
    types::Item item;
    item.push("first", 20);

    item["first"] = "strangers";
    item["second"] = "42";
    item[0] = 42;

    //limitations
    //std::cout << std::boolalpha << (item[0] == item["second"]) << std::endl;
    //You must know before hand what to do with the data! Ex.:
    std::cout << std::boolalpha << (item[0].to_int() == item["second"].to_int()) << std::endl;

    std::cout << item << std::endl;

    return true;
}

int main()
{
    tuple_tests();
    item_tests();
    return 0;
}

