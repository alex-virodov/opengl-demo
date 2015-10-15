#ifndef __IRANGE_H__
#define __IRANGE_H__

#include <vector>

class irange
{
    int m_start;
    int m_end;
public:
    class iter
    {
        int i;
    public:
        iter(int i) : i(i) {}
        int operator*() { return i; }
        void operator++() { i++; }
        bool operator!=(const iter& other) { return this->i != other.i; }

    };

    irange(int zero_to) : m_start(0), m_end(zero_to) {}
    irange(int from, int to) : m_start(from), m_end(to) {}

    template<class T>
    irange(const std::vector<T>& vec) : m_start(0), m_end(vec.size()) {}

    iter begin() { return m_start; }
    iter end()   { return m_end; }
};

#endif