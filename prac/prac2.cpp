#include <iostream>
#include <cstring>


class Person {
  public:
    explicit Person(const char *_name = "Unknown", int weight = -1) : weight(weight) {
        name = new char [strlen(_name)];
        for (size_t i = 0; i < strlen(_name); ++i)
            name[i] = _name[i];
        ++people_number;
    }
    Person(const Person &person) : weight(person.weight) {
        name = new char [sizeof(person.name)];
        for (size_t i = 0; i < sizeof(person.name); ++i)
            name[i] = person.name[i];
        ++people_number;
    }
    int get_weight() const {
        return weight;
    }
    char* get_name() const {
        return name;
    }
    static int get_people_number () {
        return people_number;
    }
    Person& operator= (const Person &person) {
        if (&person == this)
            return *this;
        weight = person.weight;
        delete[] name;
        name = new char[sizeof(person.name)];
        for (size_t i = 0; i < sizeof(person.name); ++i)
            name[i] = person.name[i];
        return *this;
    }
    int& operator()(const int _weight) {
        weight = _weight;
        return weight;
    }
    friend std::ostream& operator<<(std::ostream& stream, const Person &person);
    ~Person() {
        delete[] name;
    }
  private:
    char *name;
    int weight;
    static int people_number;
};
int Person::people_number = 0;

std::ostream& operator<<(std::ostream& stream, const Person &person) {
    stream << "name: " << person.name << "; weight: " << person.weight;
    return stream;
}

int main() {
    Person x ("Vasya", 4);
    Person y ("Kolya", 15);
    Person z = x;
    x = y;
    x = x;
    std::cout << x << '\n' << z << '\n';
    std::cout << x.get_name() << " " << Person::get_people_number()<< '\n';
    std::cout << z(20) << '\n';
    std::cout << x.get_weight() << '\n' << z.get_weight() << '\n';
}