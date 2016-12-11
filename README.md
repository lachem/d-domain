# d-domain

## Construction Constraints
```cpp
using DefaultConstructibleType = di::BasicType<int, Optional>;
using ParamConstructibleType = di::BasicType<int, Required>;
```
**Note**| By default Optional is used ```di::BasicType<int> == di::BasicType<int, Optional>```

## Injecting Code Using Mixins
BasicType does not offer much functionality apart from providing a simple value container. In order to get more from a single BasicType it is possible to use Mixins.
```cpp
struct Name
{
  static const char* name() { return "SomeName"; }
};
using StaticallyNamedType = di::BasicType<int, Mixin<Name>>;
assert(std::string("SomeName") == StaticallyNamedType::name());
assert(std::string("SomeName") == StaticallyNamedType(20).name());
```

## Unique Types With Same Signature
In order to define two different types with same signature (T1 != T2), it is possible to use a dummy Mixin type.
```cpp
struct Type1Tag{}; using T1 = di::BasicType<int, Mixin<T1Tag>>;
struct Type2Tag{}; using T2 = di::BasicType<int, Mixin<T2Tag>>;
static_assert(!std::is_same<T1,T2>::value, "");
```

## License
Copyright Adam Lach 2016. Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).
