#include "utils/ft_array.hpp"

namespace ft {

array::array(void) {}

array::array(std::vector<std::string> const& argc) : m_argc(argc) {}

array::array(array const& src) { *this = src; }

array::~array(void) {}

array& array::operator=(array const& rhs) {
    if (this != &rhs) { m_argc = rhs.m_argc; }
    return *this;
}

array::operator char**(void) const {
    char** arr = new char*[m_argc.size() + 1];

    for (size_t i = 0; i < m_argc.size(); ++i) {
        arr[i] = new char[m_argc[i].size() + 1];
        strcpy(arr[i], m_argc[i].c_str());
    }
    arr[m_argc.size()] = NULL;
    return (arr);
}

void array::erase(char** arr) {
    for (int i = 0; arr[i]; i++) { delete[] arr[i]; }
    delete[] arr;
}

} // namespace ft
