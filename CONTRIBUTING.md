# Contributing to CPFin

Thank you for considering contributing to CPFin! We welcome contributions from everyone. Here’s how you can get involved:

## How to Contribute

### 1. Reporting Issues

If you find a bug or have a feature request, please follow these steps:

1. **Check the [issue tracker](https://github.com/Angus-Toms/cpp_finance/issues)** to see if the issue has already been reported.
2. **Create a new issue** if your problem or feature request isn’t listed. Be sure to provide as much detail as possible, including:
   - A description of the problem or feature
   - Steps to reproduce the issue (if applicable)
   - Any relevant code or logs

### 2. Submitting Code

To contribute code, follow these steps:

1. **Fork the repository** on GitHub by clicking the “Fork” button at the top right of the page.
2. **Clone your fork** to your local machine:
```sh
git clone https://github.com/your-username/CPFin.git
```
3. **Create a new branch** for your changes:
```sh
git checkout -b your-branch-name
```
4. **Make your changes** and commit them:
```sh
git add .
git commit -m "A brief description of your changes"
```
5. **Push you changes** to your fork:
```sh
git push origin your-branch-name
```
6. **Create a pull request (PR)** from your fork's branch to the `main` branch:
   - Go to the [pull request page](https://github.com/Angus-Toms/CPFin/pulls) of the main branch.
   - Click "New pull request".
   - Select your branch and compare it with `main`.
   - Provide a detailed description of your changes and submit the pull request.

### 3. Code Style and Guidelines 
To ensure consistency across the codebase, please follow these guidelines:
- **Use the C++ 17 standard** for all new code
- **Write clear and concise commit messages**
- **Follow the existing codestyle** and conventions used in the project
- **Include tests** for new features if applicable

### 4. Testing 
Before submitting a PR, make sure your changes do not break existing functionality:
- **Run the existing tests** to verify everything works as expected.
- **Add new tests** if you are introducing new features or fixing bugs.
  
To run tests, use:
```sh
make test
```

### Thank You!
I appreciate your interest in contributing to CPFin. If you have any questions or need assistance, feel free to open an issue or contact me.
