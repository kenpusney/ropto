#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <cassert>

#include "ropto.hpp"

using namespace ropto;

struct employee
{
    int number;
    std::string name;
    double salary;
};

struct department
{
    std::string title;
    std::vector<employee> people;
};

template<>
class serializer<employee>
{
public:
    static employee from_bytes(byte_stream& stream)
    {
        employee value;
        stream >> value.number >> value.name >> value.salary;
        return value;
    }
    
    static void to_bytes(const employee& value, byte_stream& stream)
    {
        stream << value.number << value.name << value.salary;
    }
};
template<>
class serializer<department>
{
public:
    static department from_bytes(byte_stream& stream)
    {
        department value{};
        read(stream, value.people, value.title);
        return value;
    }
    
    static void to_bytes(const department& value, byte_stream& stream)
    {
        write(stream, value.people, value.title);
    }
};

int main(int argc, const char * argv[]) {
    
    byte_stream stream;
    
    department x {
        "TechDept",
        {
            {1, "Kimmy", 1200.00},
            {2, "Joey", 1500.00},
            {3, "Angi", 1700.00}
        }
    };
    
    write(x, stream);
    
    auto base64 = base64_encode(stream.iterate());
    
    std::cout<<base64<<std::endl;
    
    byte_stream decoded;
    
    decoded.iterate() = base64_decode(base64);
    
    auto y = read<department>(stream);
    
    std::cout << y.title << " => ";
    for (auto people: y.people)
        std::cout  << people.name << ", ";
    std::cout << std::endl;
}
