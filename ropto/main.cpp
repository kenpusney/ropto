#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <cassert>

#include "ropto.hpp"
#include "Service.hpp"

using namespace ropto;

struct employee
{
    static constexpr unsigned int type_id = 0x01;
    int number;
    std::string name;
    double salary;
};

struct department
{
    static constexpr unsigned int type_id = 0x02;
    optional<std::string> title;
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
        {},
        {
            {1, "Kimmy", 1200.00},
            {2, "Joey", 1500.00}
        }
    };
    
    employee emp {3, "Angi", 1700.00};
    
    auto service = make_service<employee, department>([x](const employee& emp, department& dept)
                 {
                     dept = x;
                     dept.people.push_back(emp);
                 });
    
    auto message = service->process(make_message(emp));
    
    auto dept = read<department>(*message.stream());
    
    for (auto emp: dept.people) {
        std::cout << emp.name << std::endl;
    }
}
