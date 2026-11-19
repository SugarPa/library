#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <string>

template <typename T>
struct is_smart_pointer : std::false_type {};

template <typename T, typename Deleter>
struct is_smart_pointer<std::unique_ptr<T, Deleter>> : std::true_type {};

template <typename T>
struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};

template <typename T>
inline constexpr bool is_smart_pointer_v = is_smart_pointer<std::remove_cvref_t<T>>::value;

template <typename T>
void print_collection_element(std::ostream& os, const T& item) {
    if constexpr (std::is_pointer_v<T> || is_smart_pointer_v<T>) {
        if (item) {
            os << *item;
        }
        else {
            os << "[nullptr]";
        }
    }
    else {
        os << item;
    }
}

template <typename T>
class Collection {
private:
    std::vector<T> items;

public:
    using value_type = T;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    Collection() = default;

    void add(const T& item) {
        items.push_back(item);
    }

    void add(T&& item) {
        items.push_back(std::move(item));
    }

    bool remove_at(std::size_t index) {
        if (index >= items.size()) {
            return false;
        }
        items.erase(items.begin() + static_cast<std::ptrdiff_t>(index));
        return true;
    }

    template <typename Predicate>
    std::size_t remove_if(Predicate pred) {
        std::size_t before = items.size();
        std::erase_if(items, pred);
        return before - items.size();
    }

    T& at(std::size_t index) {
        if (index >= items.size()) {
            throw std::out_of_range("Collection::at: индекс выходит за пределы коллекции");
        }
        return items[index];
    }

    const T& at(std::size_t index) const {
        if (index >= items.size()) {
            throw std::out_of_range("Collection::at: индекс выходит за пределы коллекции");
        }
        return items[index];
    }

    T& operator[](std::size_t index) {
        return items[index];
    }

    const T& operator[](std::size_t index) const {
        return items[index];
    }

    template <typename Key, typename KeySelector>
    T* get_by_key(const Key& key, KeySelector selector) {
        for (auto& item : items) {
            if (selector(item) == key) {
                return &item;
            }
        }
        return nullptr;
    }

    template <typename Key, typename KeySelector>
    const T* get_by_key(const Key& key, KeySelector selector) const {
        for (const auto& item : items) {
            if (selector(item) == key) {
                return &item;
            }
        }
        return nullptr;
    }

    template <typename Predicate>
    T* find_if(Predicate pred) {
        auto it = std::ranges::find_if(items, pred);
        return (it != items.end()) ? &(*it) : nullptr;
    }

    template <typename Predicate>
    const T* find_if(Predicate pred) const {
        auto it = std::ranges::find_if(items, pred);
        return (it != items.end()) ? &(*it) : nullptr;
    }

    template <typename Predicate>
    bool contains_if(Predicate pred) const {
        return std::ranges::any_of(items, pred);
    }

    std::size_t size() const noexcept {
        return items.size();
    }

    bool empty() const noexcept {
        return items.empty();
    }

    void clear() noexcept {
        items.clear();
    }

    iterator begin() noexcept { return items.begin(); }
    iterator end() noexcept { return items.end(); }
    const_iterator begin() const noexcept { return items.begin(); }
    const_iterator end() const noexcept { return items.end(); }

    void print(std::ostream& os = std::cout, const std::string& header = "Содержимое коллекции") const {
        os << "\n--- " << header << " (всего: " << items.size() << ") ---\n";
        if (items.empty()) {
            os << "Коллекция пуста.\n";
            return;
        }
        for (std::size_t i = 0; i < items.size(); ++i) {
            os << "[" << i << "] ";
            print_collection_element(os, items[i]);
            os << '\n';
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Collection<T>& collection) {
        collection.print(os);
        return os;
    }
};

template <typename T, typename Extractor, typename Value>
std::vector<const T*> find_by_field(const Collection<T>& collection, Extractor extractor, const Value& target) {
    std::vector<const T*> result;
    for (const auto& item : collection) {
        if (extractor(item) == target) {
            result.push_back(&item);
        }
    }
    return result;
}

template <typename T, typename Predicate>
std::vector<const T*> find_all_by_criteria(const Collection<T>& collection, Predicate pred) {
    std::vector<const T*> result;
    for (const auto& item : collection) {
        if (pred(item)) {
            result.push_back(&item);
        }
    }
    return result;
}

template <typename T, typename Predicate, typename Action>
std::size_t process_by_criteria(Collection<T>& collection, Predicate pred, Action action) {
    std::size_t count = 0;
    for (auto& item : collection) {
        if (pred(item)) {
            action(item);
            ++count;
        }
    }
    return count;
}