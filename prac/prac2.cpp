#include <iostream>
#include <cstring>


class Person {
  public:
    explicit Person(const char *_name = "Unknown", int weight = -1) : weight(weight) {
        name = new char [strlen(_name) + 1];
        stpcpy(name, _name);
        ++people_number;
    }
    Person(const Person &person) : weight(person.weight) {
        name = new char [strlen(person.name) + 1];
        strcpy(name, person.name);
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
        name = new char[strlen(person.name) + 1];
        strcpy(name, person.name);
        return *this;
    }
    int& operator()(const int _weight) {
        weight = _weight;
        return weight;
    }
    friend std::ostream& operator<<(std::ostream& stream, const Person &person);
    ~Person(){
        delete[] name;
    };
    virtual void do_work() const = 0;

  protected:
    char *name;
    int weight;
    static int people_number;
};
int Person::people_number = 0;

std::ostream& operator<<(std::ostream& stream, const Person &person) {
    stream << "name: " << person.name << "; weight: " << person.weight;
    return stream;
}


class Student : public Person {
  public:
    explicit Student(const char *_name = "Unknown", int weight = -1) : Person(_name, weight) {};
    void do_work() const override {
        std::cout << this -> get_name() << " is studying\n";
    }

};


int main() {
    Student x ("Vasya", 4);
    Student y ("Kolya", 15);
    Student z = x;
    x = y;
    x = x;
    std::cout << y << '\n' << z << '\n';
    std::cout << x.get_name() << " " << Person::get_people_number()<< '\n';
    std::cout << z(20) << '\n';
    std::cout << x.get_weight() << '\n' << z.get_weight() << '\n';
    x.do_work();
}