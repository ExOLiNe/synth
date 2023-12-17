#pragma once

#include <vector>

template<typename Parent, typename ...Children>
class polymorphic_readonly_array {
public:
    template<typename Container>
    explicit polymorphic_readonly_array(Container&& container)
        : align(max_size<Children...>::value()), totalElements(container.size()), capacity(align * totalElements) {
        rawData = new char[capacity];
        swapData<decltype(container), true>(std::forward<Container>(container));
    }

    polymorphic_readonly_array(const polymorphic_readonly_array& other)
        : align(other.align), totalElements(other.totalElements) {
        align = other.align;
        totalElements = other.totalElements;
        capacity = align * totalElements;
        rawData = new char[capacity];
        swapData(other.vector);
    }

    polymorphic_readonly_array(polymorphic_readonly_array&& other) noexcept
        : align(other.align), totalElements(other.totalElements), capacity(other.capacity), vector(other.vector), rawData(other.rawData) {
    }

    polymorphic_readonly_array& operator=(const polymorphic_readonly_array& other) {
        if (this != &other) {
            if (capacity < other.totalElements * other.align) {
                delete[] rawData;
                totalElements = other.totalElements;
                align = other.align;
                capacity = other.totalElements * other.align;
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

    Parent* operator[](size_t index) {
        return vector[index] ;
    }

    [[nodiscard]]
    size_t size() const {
        return vector.size();
    }
private:
    size_t align;
    size_t totalElements;
    size_t capacity;
    std::vector<Parent*> vector;
    char* rawData;

    template<typename Container, bool deletePointersData = false>
    void swapData(Container&& container) {
        vector.clear();
        vector.reserve(totalElements);
        for (size_t i = 0; i < totalElements; ++i) {
            std::memcpy(static_cast<char*>(rawData) + align * i, reinterpret_cast<char*>(container[i]), sizeof(*container[i]));
            auto* element = reinterpret_cast<Parent*>(static_cast<char*>(rawData) + align * i);
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