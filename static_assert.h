// static_assert() has some issues...
// when static_assert was introduced it required a boolean condition followed by a string literal desrciption, like so:
// static_assert(false, "");
// static_assert(false, "this should be true");
//
// which would produce the following error in visual c++ (2013, 2015, 2017)
// file.cpp(1): error C2338: 
// file.cpp(1): error C2338: this should be true
//
// later c++17 allowed the description to be optional
//
// static_assert(false);
// --> file.cpp(1): error C2338: static assertion failed
// 
// so very helpful!

// what would be nice is if the compiler included the expression in the error message:
// static_assert(1 != 1);
// --> file.cpp(1): error C2338: 1 != 1, static assertion failed
//
// or:
//
// static_assert(1 != 1, "can't you be yourself?");
// --> file.cpp(1): error C2338: 1 != 1, can't you be yourself?
//
// or better yet:
// static_assert(1 != 1, "can't you be yourself?");
// --> file.cpp(1): error C2338: static_assert(1 != 1, "can't you be yourself?")
//
// writing cross platform software targeting pre c++17 support it would be nice to allow the shoter form 
// of static_assert, it would also be nice to get good error messages...can it be done?
//
// first attempt:
// #define static_assert(condition, ...) static_assert(condition, "static_assert(" #condition "," __VA_ARGS__ ")")
//
// this almost works until you try an ddo the following:
//
//template<class A, class B> struct pair { enum { value = sizeof(A) + sizeof(B) }; };
// static_assert(pair<int,int>::value < sizeof(int), "error");
//
// the problem is macros don't know about template <>'s! so it treats it as:
//
// condition = pair<int
// __VA_ARGS__ = int>::value < sizeof(int), "error"
//
// what we need is a way to turn condition, "optional string" into just condition, at compile time...remember vs2013 doesn't
// support constexpr so can't call any functions...otherwise we could do something like:
//
// constexpr bool first(bool condition, const char* description = nullptr) { return condition; }
// #define static_assert(...) static_assert(first(__VA_ARGS__), "static_assert(" #__VA_ARGS__ ")")
//
// initalizer lists to the rescue...enjoy

struct static_assert_helper {
    bool condition;
    const char* description;
};

#define static_assert(...) static_assert(static_assert_helper{ __VA_ARGS__ }.condition, #__VA_ARGS__)
