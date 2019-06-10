# Compile time class names in gcc

```cpp
#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

template<char Character>
constexpr size_t nameLength(size_t length)
{
	static_assert(
		('a' <= Character && Character <= 'z') || 
		('A' <= Character && Character <= 'Z') || 
		('_' == Character), 
		"bad name character, only [a-zA-Z_] are allowed");
	
	return (length > 0 && 'A' <= Character && Character <= 'Z') ? length + 2 : length + 1;
}

template<char First, char Second, char... Rest>
constexpr size_t nameLength(size_t length)
{
	return nameLength<Second, Rest...>(nameLength<First>(length));
}

template<typename Dist1, typename Dist2>
struct CombinedDistTag
{
	static const char* clsn() { return Dist1::clsn(); }
	static const char* name() { return Dist2::clsn(); }
};

template<char... String>
class DistTag
{
	static constexpr size_t length_clsn = sizeof...(String) + 1;
	static constexpr size_t length_name = nameLength<String...>(0) + 1;
	
public:
	static const char* clsn() {
		static const char str[] = {String..., '\0'};
		return str;
	}
	
	static const char* name() {
		static const char* str = resolveContextName();
		return str;
	}
	
	template<char... Other>
	friend CombinedDistTag<DistTag<String...>, DistTag<Other...>> operator,(DistTag<String...>, DistTag<Other...>)
	{
		return {};
	}

private:
	static const char* resolveContextName()
	{
		static char str[length_name] = {String..., '\0'};
		
		auto cls = clsn();
		for(size_t i = 0, offset = 0; i < length_clsn; ++i)
		{
			if(std::isupper(cls[i]) && i>0)
			{
				str[i+(offset++)] = '_';
			}
			str[i+offset] = char(std::tolower(cls[i]));
		}
		str[length_name - 1] = '\0';
		
		return str;
	}
};

template<typename CharT, CharT ... String>
constexpr DistTag<String...> operator""_tag() {
  static_assert(std::is_same<CharT, char>(), "can only use printf on narrow strings");
  return {};
}

template<typename Tag>
struct Property : Tag
{
};

int main() {
	std::cout << Property<decltype("ClassName"_tag)>::name() << std::endl;
	std::cout << Property<decltype("ClassName"_tag)>::clsn() << std::endl;
	std::cout << Property<decltype("ClassName"_tag,"override_name"_tag)>::name() << std::endl;
	std::cout << Property<decltype("ClassName"_tag,"override_name"_tag)>::clsn() << std::endl;
    return 0;
}
```

# Cheap member detection mechanism
```cpp
#include <iostream>
#include <string>
#include <utility>

struct TypeWithAlias
{
	using type = int;
};

struct TypeNoAlias 
{
};

template<typename T>
auto has_type(T) -> decltype(typename T::type{})
{
	return {};
}

auto has_type(...) -> std::string
{
	return "fallback";
}

int main() 
{
	std::cout << "int: " << has_type(TypeWithAlias{}) << std::endl;
	std::cout << "string: " << has_type(TypeNoAlias{}) << std::endl;
	return 0;
}
```
