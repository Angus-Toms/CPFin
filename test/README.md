# CPFin Tests

This directory contains a number of [gtest](https://github.com/google/googletest) unit tests. To run them, navigate to the root directory and call:
```sh
make test
```

When adding tests, ensure you edit the [`CMakeLists.txt`](https://github.com/Angus-Toms/CPFin/blob/main/test/CMakeLists.txt) file to include them. 

All tests are automatically run upon any push to the main branch, using [GitHub Actions](https://docs.github.com/en/actions).