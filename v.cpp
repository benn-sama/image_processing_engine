#include <iostream>

class Animal {
    public:
        // declaring the func as virtual
        virtual void makeSound() const { std::cout << "Generic animal sound lammeeee\n"; }
        virtual ~Animal() = default; // essential: ALWAYS declare a virtual destructor
};

class Dog : public Animal {
    public:
        void makeSound() const override { std::cout << "Woof!\n";}
};

void printSound(const Animal& animalRef) {
    animalRef.makeSound(); // dynamically binds at runtime
}

int main() {
    Animal* ptr = new Dog();
    ptr->makeSound(); // outputs: "woof" calls derived method
    // or this works also
    Dog dog;
    printSound(dog);
    delete ptr;
}