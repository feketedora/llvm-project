// RUN: %check_clang_tidy %s modernize-use-ranges %t

// #include <ranges>
namespace std {
template <class T>
T &&declval() noexcept;
template <class _Ip, class _Op>
_Op copy(_Ip first, _Ip last, _Op out) { return out; }
template <class C>
typename C::const_iterator begin(const C &c) { return c.begin(); }
template <class C>
typename C::const_iterator end(const C &c) { return c.end(); }
template <class T>
class vector {
public:
  using const_iterator = const T *;
  const_iterator begin() const {
    return {};
  }
  const_iterator end() const {
    return {};
  };
};
struct back_insert_iterator {
};
template <class C>
back_insert_iterator back_inserter(C &) {
  return {};
}
namespace ranges {
template <class T>
using iterator_t = decltype(std::begin(declval<T &>()));
template <class R>
using safe_iterator_t = iterator_t<R>;
template <class I, class O>
struct copy_result {
  I in;
  O out;
};
template <class I, class S, class O>
constexpr copy_result<I, O> copy(I first, S last, O result) {
  return {last, result};
}
template <class R, class O>
constexpr copy_result<safe_iterator_t<R>, O> copy(R &&r, O result) {
  return {std::end(r), result};
}
} // namespace ranges
} // namespace std

void std_copy_1() {
  std::vector<int> v1;
  std::vector<int> out;
  std::copy(v1.begin(), v1.end(), std::back_inserter(out));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: Consider to replace 'std::copy' by 'std::ranges::copy' [modernize-use-ranges]
  // CHECK-FIXES: std::ranges::copy(v1, std::back_inserter(out));
}

void std_copy_2() {
  std::vector<int> v2;
  std::vector<int> out;
  std::copy(std::begin(v2), std::end(v2), std::back_inserter(out));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: Consider to replace 'std::copy' by 'std::ranges::copy' [modernize-use-ranges]
  // CHECK-FIXES: std::ranges::copy(v2, std::back_inserter(out));
}

void std_copy_3() {
  std::vector<int> *v3;
  std::vector<int> out;
  std::copy(v3->begin(), v3->end(), std::back_inserter(out));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: Consider to replace 'std::copy' by 'std::ranges::copy' [modernize-use-ranges]
  // CHECK-FIXES: std::ranges::copy(*v3, std::back_inserter(out));
}

void std_copy_4() {
  std::vector<int> *v4;
  std::vector<int> out;
  std::copy(std::begin(*v4), std::end(*v4), std::back_inserter(out));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: Consider to replace 'std::copy' by 'std::ranges::copy' [modernize-use-ranges]
  // CHECK-FIXES: std::ranges::copy(*v4, std::back_inserter(out));
}

void range_1() {
  std::vector<int> vs;
  std::vector<int> out;
  std::ranges::copy(vs.begin(), vs.end(), std::back_inserter(out));
}

void range_2() {
  std::vector<int> vs;
  std::vector<int> out;
  std::ranges::copy(vs, std::back_inserter(out));
}