# Naming

- Naming of C++ public entities should match the naming of C++ standard library.
  ```cpp
  struct foo {
    float public_value;
    void public_function();
  };
  ```
- Private entities should still match C++ standard library, but adopt the
  conventions that does not involve letters (ex. `k_xxx`) from
  Google's C++ Style Guide.
  ```cpp
  class foo {
   private:
    float my_constant;
    float private_value_;
    void private_function();
  };
  ```
