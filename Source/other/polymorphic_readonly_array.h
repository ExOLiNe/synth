#pragma once

#include <vector>

template<typename Parent, typename ...Children>
class polymorphic_readonly_array {
public:
    /*explicit polymorphic_readonly_array(std::array<Parent*, total>&& array) : maxSize(max_size<Children...>::value()), totalElements(total) {
        //static_assert(is_copy_constructible<Children...>::value());

        rawData = new char[total * maxSize];
        swapData<decltype(array), true>(std::move(array));
    }*/

    template<typename Container>
    explicit polymorphic_readonly_array(Container&& container)
        : maxSize(max_size<Children...>::value()), totalElements(container.size()), capacity(maxSize * totalElements) {
        rawData = new char[capacity];
        swapData<decltype(container), true>(std::move(container));
    }

    polymorphic_readonly_array(const std::vector<Parent*>& vector)
        : maxSize(max_size<Children...>::value()), totalElements(vector.size()), capacity(maxSize * totalElements) {
        rawData = new char[capacity];
        swapData<decltype(vector), true>(vector);
    }

    polymorphic_readonly_array(const polymorphic_readonly_array& other)
        : maxSize(other.maxSize), totalElements(other.totalElements) {
        maxSize = other.maxSize;
        totalElements = other.totalElements;
        capacity = maxSize * totalElements;
        rawData = new char[capacity];
        swapData(other.vector);
    }

    polymorphic_readonly_array(polymorphic_readonly_array&& other) noexcept
        : vector(other.vector), maxSize(other.maxSize), totalElements(other.totalElements), rawData(other.rawData) {
    }

    //polymorphic_readonly_array& operator=(const polymorphic_readonly_array& other) = delete;
    polymorphic_readonly_array& operator=(const polymorphic_readonly_array& other) {
        if (this != &other) {
            if (capacity < other.totalElements * other.maxSize) {
                delete[] rawData;
                totalElements = other.totalElements;
                maxSize = other.maxSize;
                capacity = other.totalElements * other.maxSize;
                rawData = new char[capacity];
            }
            swapData(other.vector);
        }
        return *this;
    }

    polymorphic_readonly_array& operator=(polymorphic_readonly_array&& other) = delete;

    ~polymorphic_readonly_array() {
        delete[] rawData;
    }

    Parent* operator[](int index) {
        return vector[index] ;
    }

    size_t size() const {
        return vector.size();
    }
private:
    size_t maxSize;
    size_t totalElements;
    size_t capacity;
    std::vector<Parent*> vector;
    void* rawData;

    template<typename Container, bool deletePointersData = false>
    void swapData(Container&& container) {
        vector.clear();
        vector.reserve(totalElements);
        for (int i = 0; i < totalElements; ++i) {
            std::memcpy((void*)((char*) rawData + maxSize * i), container[i], sizeof(container[i]));
            Parent* element = (Parent*)((char*) rawData + maxSize * i);
            vector.push_back(element);
            if constexpr (deletePointersData) {
                delete container[i];
            }
        }
    }

    template<typename ...Types>
    struct max_size {
        static constexpr size_t value() {
            return get_max_type_size<Types...>();
        }
    private:
        template <typename T>
        static constexpr size_t get_max_type_size() {
            return sizeof(T);
        }

        template <typename T, typename Arg, typename... Args>
        static constexpr size_t get_max_type_size() {
            return max(sizeof(T), get_max_type_size<Arg, Args...>());
        }

        static constexpr size_t max(size_t v1, size_t v2) {
            return v1 > v2 ? v1 : v2;
        }
    };

    /*template<typename ...Types>
    struct is_copy_constructible {
        static constexpr bool value() {
            return _is_copy_constructible<Types...>();
        }
    private:
        template<typename T, typename... Ts>
        static constexpr bool _is_copy_constructible() {
            if constexpr (sizeof...(Ts) == 0) {
                return std::is_copy_constructible_v<T>;
            } else {
                return _is_copy_constructible<T>() && _is_copy_constructible<Ts...>();
            }
        }
    };*/
};