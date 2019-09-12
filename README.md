[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

# LucppeneQueryParser

A library for parsing a search query string into a vector of descriptive objects according to Lucene's search query syntax.

## Getting Started

### Prerequisites

This project requires a compiler with C++17 support and CMake 3.14+.

### Usage

...

## CMake options

The following CMake options are made available by this project:

- WERROR: Treat warnings as errors
- BUILD_SHARED_LIBS: Build shared libraries
- BUILD_TESTING: Build the testing tree
  - Run with `cd tests/ && ctest`
- BUILD_SAMPLES: Build sample applications
  - Run with `./bin/mytemplate.example`
- ENABLE_COVERAGE: Enable code coverage reporting with Codecov
  - Run with `make mytemplate.coverage`
- ENABLE_CLANG_FORMAT: Run clang-format on all files in src/ and include/
  - Run with `make format`
- ENABLE_ASAN: Enable Google AddressSanitizer
  - Run with `cd tests/ && ctest`
- ENABLE_USAN: Enable Google UndefinedBehaviorSanitizer
  - Run with `cd tests/ && ctest`
- ENABLE_TSAN: Enable Google ThreadSanitizer
  - Run with `cd tests/ && ctest`
- ENABLE_VALGRIND: Enable Valgrind
  - Run with `ctest -T memcheck`

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in the work by you shall be licensed as below, without any additional terms or conditions.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](tags).

## Authors

- **Patrick Cox** - [paddy74](https://github.com/paddy74)

## License

This project is licensed under the Apache 2.0 License - see the [LICENSE.md](LICENSE.md) file for details.
