#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>

using namespace std;

class ContainerException : public exception {
public:
    ContainerException(const char* message) : m_message(message) {}

    const char* what() const noexcept override {
        return m_message;
    }

private:
    const char* m_message;
};

// template class
template <typename T>
class Container
{
public:
    Container() = default;

    void addItem(T item) {
        contents.push_back(item);
    }

    void removeItem(int i) {
        contents.erase(contents.begin() + i);
    }

    int size() const {
        return contents.size();
    }

    // programmer defined exception
    T get(int i) const {
        if (i < 0 || i >= contents.size()) {
            throw ContainerException("Index out of range");
        }
        return contents.at(i);
    }

    // Operator overload for +
    void operator+ (T& item) {
        addItem(item);
    }

private:
    vector<T> contents;
};

#endif // CONTAINER_H


