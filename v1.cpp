#include <iostream>


template <typename T>
class circul_buf {
private:
    friend class circul_iterator;
    template<class Iter>
    class circul_iterator {
        friend class circul_buf;

    public:
        typedef Iter iterator_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef int difference_type;
        typedef iterator_type value_type;
        typedef iterator_type& reference;
        typedef iterator_type* pointer;

        iterator_type* value;
    private:
        circul_iterator(Iter* p) : value(p) {}

    public:
        circul_iterator(const circul_iterator& it) : value(it.value) {}

        circul_iterator& operator++() {
            if (value == (buffer + size_alloc - 1)) {
                value = buffer;
            }
            else
                value++;
        }

        bool operator==(circul_iterator const& other) const {
            return value == other.value;
        }

        bool operator!=(circul_iterator const& other) const {
            return value != other.value;
        }

    };

public:
    typedef T* pointer_t;
    typedef T value_t;



    circul_buf() {
        size_alloc = 1;
        buffer = new value_t[1];
        start = buffer;
        end = buffer;
        count_elem = 0;
    }

    circul_buf(unsigned n) {
        size_alloc = n;
        buffer = new value_t[n];
        start = buffer;
        end = buffer;
        count_elem = 0;
    }

    ~circul_buf() {
        free(buffer);
    }

    void push_back(value_t new_elem) {
        if (count_elem < size_alloc) {
            if (end == (buffer + (size_alloc * sizeof(value_t))))
                end = buffer;
            *end = new_elem;
            end++;
            count_elem++;
        }
        else {

            int transfer_st = start - buffer, transfer_end = end - buffer;
            size_alloc *= 2;
//            buffer = (value_t*)realloc(buffer, size_alloc * sizeof(value_t));

            pointer_t buffer2 = new value_t[size_alloc * 2];
            for (int i = 0; i < (size_alloc / 2); i++)
                buffer2[i] = buffer[i];
            delete[] buffer;
            buffer = buffer2;

            start = buffer + (transfer_st * sizeof(value_t));
            pointer_t temp= buffer + ((size_alloc / 2) * sizeof(value_t));
            if ((buffer + (transfer_end * sizeof(value_t))) == temp) {
                end = buffer + transfer_end;
            }
            else {
                end = temp + (transfer_end * sizeof(value_t));
                while (temp != end) {
                    *temp = *(buffer - (buffer + ((size_alloc / 2) * sizeof(value_t)) - temp));
                    temp++;
                }
            }
            *end = new_elem;
            end++;
            count_elem++;
        }

    }

    void push_front(value_t new_elem) {
        if (count_elem < size_alloc) {

        }
        else {
            int transfer_st = start - buffer, transfer_end = end - buffer;
            size_alloc *= 2;

            pointer_t buffer2 = new value_t[size_alloc * 2];
            for (int i = 0; i < (size_alloc / 2); i++)
                buffer2[i] = buffer[i];
            delete[] buffer;
            buffer = buffer2;

            start = buffer + (transfer_st * sizeof(value_t));
            pointer_t temp= buffer + ((size_alloc / 2) * sizeof(value_t));
            if ((buffer + (transfer_end * sizeof(value_t))) == temp) {
                end = buffer + transfer_end;
            }
            else {
                end = temp + (transfer_end * sizeof(value_t));
                while (temp != end) {
                    *temp = *(buffer - (buffer + ((size_alloc / 2) * sizeof(value_t)) - temp));
                    temp++;
                }
            }

        }
            if (start != buffer)
                start--;
            else
                start = buffer + (size_alloc - 1) ;

            *start = new_elem;
            count_elem++;
    }

    void pop_front() {
        if (count_elem > 0) {
            if (start == buffer + (size_alloc - 1)) {
                start = buffer;
            }
            else
                start++;

            count_elem--;
        }
    }

    void pop_back() {
        if (count_elem > 0) {
            if (end == buffer) {
                end = buffer + ((size_alloc - 1));
            }
            else
                end--;
            count_elem--;
        }
    }

    value_t get_start() {
        return *start;
    }
    value_t get_end() {
        if (end == buffer) {
            return  *(buffer + (size_alloc - 1));
        }
        else
            return *(end - 1);
    }

    typedef circul_iterator<value_t> iterator;
    typedef circul_iterator<const value_t> const_iterator;

    iterator begin() const {
        return iterator(begin);
    }
    



    value_t& operator[](const unsigned& n) {
        if (n < count_elem) {
            if (start - end > 0) {
                pointer_t end_alloc = buffer + size_alloc, end_think = start + 1;
                int first_chapter = end_alloc - start;
                if (n < ((buffer + size_alloc) - start))
                    return *(start + n);
                else
                    return *(buffer + n - (buffer + size_alloc - start));
            } else {
                return *(start + n);
            }
        }
    }

    void change_capasity(unsigned n) {

    }

    unsigned size() {
        return count_elem;
    }



private:
    static pointer_t buffer;
    static unsigned size_alloc;
    unsigned count_elem;
    pointer_t start;
    pointer_t end;
};
int main() {
    circul_buf<int> arr;
    arr.push_back(5);
    arr.push_back(6);
    arr.push_back(7);
    arr.push_back(8);
    arr.push_back(9);
    arr.push_back(10);
    arr.push_back(11);
    arr.push_back(12);
    arr.pop_front();
    arr.pop_front();
    arr.pop_front();
//    arr.pop_back();
//    arr.pop_back();
//    arr.pop_back();
//    arr.push_front(13);
//    arr.push_front(14);
//    arr.push_front(15);
//    arr.push_front(16);
    arr.push_back(13);
    arr.push_back(14);
    arr.push_back(15);


//    arr.push_back(14);


    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << " ";
    }

    return 0;
}
