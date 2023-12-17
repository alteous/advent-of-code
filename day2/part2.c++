#include <iostream>
#include <optional>
#include <string>
#include <string_view>

enum class color {
    red,
    green,
    blue,
};

int max_quantity(color c)
{
    switch (c) {
    case color::red: return 12;
    case color::green: return 13;
    case color::blue: return 14;
    default: return 0;
    }
}

template <typename T>
struct parser {
    static std::optional<T> parse(std::string_view slice);
};

template <>
struct parser<int> {
    static std::optional<int> parse(std::string_view slice) {
        auto n = 0;
        for (auto c: slice) {
            n *= 10;
            n += c - '0';
        }
        return n;
    }
};

template <>
struct parser<color> {
    static std::optional<color> parse(std::string_view slice) {
        if (slice == "red") {
            return color::red;
        } else if (slice == "green") {
            return color::green;
        } else if (slice == "blue") {
            return color::blue;
        } else {
            return std::nullopt;
        }
    }
};

class delimited {
public:
    delimited() = default;

    delimited(std::string_view slice, char delimiter)
        : m_input{slice, delimiter}
    {
    }

    class iterator {
    public:
        iterator() = default;

        iterator(std::string_view slice, char delimiter)
            : m_head{slice}
            , m_tail{""}
            , m_delimiter{delimiter}
        {
            auto position = slice.find(delimiter);
            m_head = slice.substr(0, position);
            if (position != slice.npos) {
                m_tail = slice.substr(position + 1);
            }
        }

        iterator operator++() {
            return *this = iterator{m_tail, m_delimiter};
        }

        iterator operator++(int) {
            auto previous = *this;
            ++(*this);
            return previous;
        }

        bool operator==(const iterator &other) const {
            return m_head == other.m_head && m_tail == other.m_tail;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }

        std::string_view operator*() const {
            return m_head;
        }

    private:
        std::string_view m_head{""};
        std::string_view m_tail{""};
        char m_delimiter{};
    };

    iterator begin() const {
        return iterator{m_input};
    }

    iterator end() const {
        return iterator{};
    }

private:
    iterator m_input{};
};

std::string_view trim_whitespace(std::string_view &slice)
{
    while (slice.starts_with(' '))
        slice.remove_prefix(1);
    while (slice.ends_with(' '))
        slice.remove_suffix(1);
    return slice;
}

std::string_view trim_prefix(std::string_view &slice, std::string_view prefix)
{
    if (slice.starts_with(prefix))
        slice.remove_prefix(prefix.size());
    return slice;
}

int power(std::string_view line)
{
    auto game_tokenizer = delimited{line, ':'};
    auto game_tokens = game_tokenizer.begin();

    auto game_header = *game_tokens++;
    auto game_id = *parser<int>::parse(trim_prefix(game_header, "Game "));

    auto min_green = 0;
    auto min_red = 0;
    auto min_blue = 0;

    auto frames = *game_tokens++;
    for (auto frame: delimited{frames, ';'}) {
        for (auto outcome: delimited{frame, ','}) {
            auto outcome_tokenizer = delimited{trim_whitespace(outcome), ' '};
            auto outcome_tokens = outcome_tokenizer.begin();
            auto outcome_quantity = *parser<int>::parse(*outcome_tokens++);
            auto outcome_color = *parser<color>::parse(*outcome_tokens++);
            switch (outcome_color) {
            case color::red:
                min_red = std::max(min_red, outcome_quantity);
                break;
            case color::green:
                min_green = std::max(min_green, outcome_quantity);
                break;
            case color::blue:
                min_blue = std::max(min_blue, outcome_quantity);
                break;
            }
        }
    }

    return min_red * min_green * min_blue;
}

int main()
{
    auto total = 0;
    auto line = std::string{};
    while (std::getline(std::cin, line)) {
        total += power(line);
    }
    std::cout << total << '\n';
}
