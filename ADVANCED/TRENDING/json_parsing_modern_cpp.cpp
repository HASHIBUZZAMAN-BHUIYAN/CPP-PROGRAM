// ADVANCED TRENDING: JSON Parsing in Modern C++
//
// This file demonstrates JSON concepts two ways:
//
// WAY 1 (default, no deps): Hand-rolled JSON parser — shows how JSON parsing works,
//   teaches recursive descent parsing, variant types, and visitor pattern.
//
// WAY 2 (with nlohmann/json): The industry-standard header-only library.
//   Download nlohmann/json.hpp from: https://github.com/nlohmann/json/releases
//   Place json.hpp next to this file, then recompile with -DUSE_NLOHMANN.
//
// Compile (no deps):
//   g++ -std=c++17 -Wall -O2 -o json_parsing_modern_cpp.exe json_parsing_modern_cpp.cpp
// Compile (with nlohmann):
//   g++ -std=c++17 -Wall -O2 -DUSE_NLOHMANN -o json_parsing_modern_cpp.exe json_parsing_modern_cpp.cpp

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <cassert>
#include <cmath>

#ifdef USE_NLOHMANN
  #include "json.hpp"
  using json = nlohmann::json;
#endif

// ============================================================
// Minimal JSON value type using std::variant (C++17)
// ============================================================
struct JsonNull {};

struct JsonValue;
using JsonArray  = std::vector<JsonValue>;
using JsonObject = std::map<std::string, JsonValue>;

struct JsonValue {
    using Data = std::variant<JsonNull, bool, double, std::string, JsonArray, JsonObject>;
    Data data;

    JsonValue()                      : data(JsonNull{}) {}
    JsonValue(std::nullptr_t)        : data(JsonNull{}) {}
    JsonValue(bool b)                : data(b) {}
    JsonValue(double d)              : data(d) {}
    JsonValue(int i)                 : data(static_cast<double>(i)) {}
    JsonValue(const std::string& s)  : data(s) {}
    JsonValue(std::string&& s)       : data(std::move(s)) {}
    JsonValue(const char* s)         : data(std::string(s)) {}
    JsonValue(JsonArray arr)         : data(std::move(arr)) {}
    JsonValue(JsonObject obj)        : data(std::move(obj)) {}

    bool is_null()   const { return std::holds_alternative<JsonNull>(data); }
    bool is_bool()   const { return std::holds_alternative<bool>(data); }
    bool is_number() const { return std::holds_alternative<double>(data); }
    bool is_string() const { return std::holds_alternative<std::string>(data); }
    bool is_array()  const { return std::holds_alternative<JsonArray>(data); }
    bool is_object() const { return std::holds_alternative<JsonObject>(data); }

    bool              get_bool()   const { return std::get<bool>(data); }
    double            get_number() const { return std::get<double>(data); }
    const std::string& get_string() const { return std::get<std::string>(data); }
    const JsonArray&  get_array()  const { return std::get<JsonArray>(data); }
    const JsonObject& get_object() const { return std::get<JsonObject>(data); }
    JsonArray&        get_array()        { return std::get<JsonArray>(data); }
    JsonObject&       get_object()       { return std::get<JsonObject>(data); }

    // Array/object access by index/key
    const JsonValue& operator[](size_t i)          const { return get_array()[i]; }
    const JsonValue& operator[](const std::string& k) const { return get_object().at(k); }
    JsonValue& operator[](const std::string& k)          { return get_object()[k]; }

    bool contains(const std::string& k) const {
        return is_object() && get_object().count(k);
    }
};

// ============================================================
// Serializer (JsonValue -> JSON string)
// ============================================================
std::string to_json_string(const JsonValue& v, int indent = 0) {
    const std::string pad(indent * 2, ' ');
    const std::string pad2((indent + 1) * 2, ' ');

    return std::visit([&](const auto& x) -> std::string {
        using T = std::decay_t<decltype(x)>;
        if constexpr (std::is_same_v<T, JsonNull>)        return "null";
        else if constexpr (std::is_same_v<T, bool>)       return x ? "true" : "false";
        else if constexpr (std::is_same_v<T, double>) {
            if (x == std::floor(x) && std::abs(x) < 1e15)
                return std::to_string(static_cast<long long>(x));
            std::ostringstream oss; oss << std::setprecision(10) << x;
            return oss.str();
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            std::string out = "\"";
            for (char c : x) {
                if (c == '"')  out += "\\\"";
                else if (c == '\\') out += "\\\\";
                else if (c == '\n') out += "\\n";
                else out += c;
            }
            return out + "\"";
        }
        else if constexpr (std::is_same_v<T, JsonArray>) {
            if (x.empty()) return "[]";
            std::string out = "[\n";
            for (size_t i = 0; i < x.size(); ++i) {
                out += pad2 + to_json_string(x[i], indent + 1);
                if (i + 1 < x.size()) out += ",";
                out += "\n";
            }
            return out + pad + "]";
        }
        else {  // JsonObject
            if (x.empty()) return "{}";
            std::string out = "{\n";
            size_t i = 0;
            for (const auto& [k, val] : x) {
                out += pad2 + "\"" + k + "\": " + to_json_string(val, indent + 1);
                if (++i < x.size()) out += ",";
                out += "\n";
            }
            return out + pad + "}";
        }
    }, v.data);
}

// ============================================================
// Parser (JSON string -> JsonValue) — recursive descent
// ============================================================
class JsonParser {
    const std::string& src;
    size_t pos{0};

    void skip_ws() {
        while (pos < src.size() && std::isspace(static_cast<unsigned char>(src[pos]))) ++pos;
    }

    char peek() { skip_ws(); return pos < src.size() ? src[pos] : '\0'; }
    char next() { skip_ws(); return pos < src.size() ? src[pos++] : '\0'; }

    void expect(char c) {
        char got = next();
        if (got != c) throw std::runtime_error(
            std::string("Expected '") + c + "' got '" + got + "' at pos " + std::to_string(pos));
    }

    std::string parse_string() {
        expect('"');
        std::string result;
        while (pos < src.size() && src[pos] != '"') {
            if (src[pos] == '\\') {
                ++pos;
                switch (src[pos]) {
                    case '"':  result += '"';  break;
                    case '\\': result += '\\'; break;
                    case 'n':  result += '\n'; break;
                    case 't':  result += '\t'; break;
                    default:   result += src[pos];
                }
            } else {
                result += src[pos];
            }
            ++pos;
        }
        expect('"');
        return result;
    }

    double parse_number() {
        size_t start = pos;
        if (pos < src.size() && src[pos] == '-') ++pos;
        while (pos < src.size() && std::isdigit(static_cast<unsigned char>(src[pos]))) ++pos;
        if (pos < src.size() && src[pos] == '.') {
            ++pos;
            while (pos < src.size() && std::isdigit(static_cast<unsigned char>(src[pos]))) ++pos;
        }
        if (pos < src.size() && (src[pos] == 'e' || src[pos] == 'E')) {
            ++pos;
            if (pos < src.size() && (src[pos] == '+' || src[pos] == '-')) ++pos;
            while (pos < src.size() && std::isdigit(static_cast<unsigned char>(src[pos]))) ++pos;
        }
        return std::stod(src.substr(start, pos - start));
    }

    JsonValue parse_value() {
        char c = peek();
        if (c == '"') return parse_string();
        if (c == '{') return parse_object();
        if (c == '[') return parse_array();
        if (c == 't') { pos += 4; return true; }   // true
        if (c == 'f') { pos += 5; return false; }  // false
        if (c == 'n') { pos += 4; return nullptr; } // null
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) return parse_number();
        throw std::runtime_error(std::string("Unexpected char '") + c + "'");
    }

    JsonValue parse_object() {
        expect('{');
        JsonObject obj;
        skip_ws();
        if (peek() == '}') { ++pos; return obj; }
        while (true) {
            std::string key = parse_string();
            expect(':');
            obj[key] = parse_value();
            skip_ws();
            if (peek() == '}') { ++pos; break; }
            expect(',');
        }
        return obj;
    }

    JsonValue parse_array() {
        expect('[');
        JsonArray arr;
        skip_ws();
        if (peek() == ']') { ++pos; return arr; }
        while (true) {
            arr.push_back(parse_value());
            skip_ws();
            if (peek() == ']') { ++pos; break; }
            expect(',');
        }
        return arr;
    }

public:
    explicit JsonParser(const std::string& s) : src(s) {}
    JsonValue parse() { return parse_value(); }
};

JsonValue parse_json(const std::string& s) {
    return JsonParser(s).parse();
}

// ============================================================
// Demo 1: Build and serialise JSON programmatically
// ============================================================
void demo_build_json() {
    std::cout << "\n=== 1. Build JSON Programmatically ===\n";

    // C++ object model for a person
    struct Person { std::string name; int age; std::vector<std::string> hobbies; };

    auto person_to_json = [](const Person& p) -> JsonValue {
        JsonArray hobbies_arr;
        for (const auto& h : p.hobbies) hobbies_arr.push_back(h);
        JsonObject obj;
        obj["name"]    = p.name;
        obj["age"]     = p.age;
        obj["active"]  = true;
        obj["score"]   = 98.5;
        obj["address"] = nullptr;
        obj["hobbies"] = std::move(hobbies_arr);
        return obj;
    };

    Person alice{"Alice", 30, {"C++", "robotics", "chess"}};
    JsonValue json = person_to_json(alice);
    std::cout << to_json_string(json) << "\n";
}

// ============================================================
// Demo 2: Parse JSON string
// ============================================================
void demo_parse_json() {
    std::cout << "\n=== 2. Parse JSON String ===\n";

    const std::string input = R"({
        "name": "Bob",
        "age": 25,
        "scores": [88, 92, 95, 79],
        "active": true,
        "address": null,
        "contact": {
            "email": "bob@example.com",
            "phone": "555-0100"
        }
    })";

    JsonValue v = parse_json(input);

    std::cout << "name:   " << v["name"].get_string() << "\n";
    std::cout << "age:    " << static_cast<int>(v["age"].get_number()) << "\n";
    std::cout << "active: " << (v["active"].get_bool() ? "true" : "false") << "\n";
    std::cout << "email:  " << v["contact"]["email"].get_string() << "\n";

    const auto& scores = v["scores"].get_array();
    double total = 0;
    for (const auto& s : scores) total += s.get_number();
    std::cout << "avg score: " << (total / scores.size()) << "\n";

    std::cout << "address is null: " << (v["address"].is_null() ? "yes" : "no") << "\n";
}

// ============================================================
// Demo 3: Round-trip (parse -> modify -> re-serialize)
// ============================================================
void demo_roundtrip() {
    std::cout << "\n=== 3. Round-Trip: Parse, Modify, Re-Serialize ===\n";

    const std::string cfg_json = R"({"host":"localhost","port":8080,"debug":false,"tags":["api","v1"]})";
    JsonValue cfg = parse_json(cfg_json);

    // Modify
    cfg.get_object()["port"]  = 9090.0;
    cfg.get_object()["debug"] = true;
    cfg.get_object()["tags"].get_array().push_back("updated");

    std::cout << to_json_string(cfg) << "\n";
}

// ============================================================
// Demo 4: std::variant visitor pattern
// ============================================================
void demo_variant_visitor() {
    std::cout << "\n=== 4. std::variant + std::visit (type-safe dispatch) ===\n";

    // JsonValue::data is a variant — visit handles each type
    auto type_name = [](const JsonValue& v) -> std::string {
        return std::visit([](const auto& x) -> std::string {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, JsonNull>)   return "null";
            if constexpr (std::is_same_v<T, bool>)       return "bool";
            if constexpr (std::is_same_v<T, double>)     return "number";
            if constexpr (std::is_same_v<T, std::string>)return "string";
            if constexpr (std::is_same_v<T, JsonArray>)  return "array";
            if constexpr (std::is_same_v<T, JsonObject>) return "object";
            return "unknown";
        }, v.data);
    };

    std::vector<JsonValue> values = {nullptr, true, 42.0, "hello", JsonArray{}, JsonObject{}};
    for (const auto& v : values)
        std::cout << "  type: " << type_name(v) << "\n";

    std::cout << "\nstd::variant enforces exhaustive type matching at compile time\n";
    std::cout << "No runtime type_id or dynamic_cast needed\n";
}

#ifdef USE_NLOHMANN
void demo_nlohmann() {
    std::cout << "\n=== nlohmann/json Demo ===\n";

    json j = {
        {"name", "Charlie"},
        {"age",  35},
        {"tags", {"cpp", "json", "modern"}},
        {"cfg",  {{"debug", true}, {"timeout", 5000}}}
    };

    std::cout << j.dump(2) << "\n";
    std::cout << "name: " << j["name"].get<std::string>() << "\n";
    std::cout << "age:  " << j["age"].get<int>() << "\n";

    // Parse string
    std::string s = R"({"x": 1.5, "y": [2, 3, 4]})";
    json parsed = json::parse(s);
    std::cout << "x=" << parsed["x"].get<double>()
              << " y[1]=" << parsed["y"][1].get<int>() << "\n";
}
#endif

int main() {
    std::cout << "=== JSON Parsing in Modern C++ ===\n";
    std::cout << "Using self-contained variant-based parser (no external deps)\n";

    demo_build_json();
    demo_parse_json();
    demo_roundtrip();
    demo_variant_visitor();

#ifdef USE_NLOHMANN
    demo_nlohmann();
#else
    std::cout << "\n=== Using nlohmann/json ===\n";
    std::cout << "Recompile with -DUSE_NLOHMANN after placing json.hpp here:\n";
    std::cout << "  " << __FILE__ << " (same directory)\n";
    std::cout << "Download: https://github.com/nlohmann/json/releases\n";
    std::cout << "nlohmann/json is the most popular C++ JSON library.\n";
    std::cout << "Its API is similar to the hand-rolled version above.\n";
#endif

    return 0;
}
