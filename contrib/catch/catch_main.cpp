#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE
#include <catch.hpp>

// start ignore out of memory errors in ASAN
#ifndef __has_feature
#define __has_feature(feature) 0
#endif

#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#ifdef __cplusplus
extern "C"
#endif
const char* __asan_default_options() {
  return "allocator_may_return_null=1";
}
#endif
// end ignore out of memory errors in ASAN
