# Velvet - A Physics/Game Engine

Velvet is an open-source physics and game engine written in C++. It's designed to be beginner-friendly, making application development with Velvet easy and enjoyable.
## Table of Contents
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Generate Project Files](#generate-project-files)
- [Building Velvet](#building-velvet)
- [Contributing](#contributing-to-velvet)
- [License](#license)
## Dependencies
- [spdlog @ ddce421](https://github.com/gabime/spdlog) in Velvet/vendor/spdlog
- [GLFW @ d2232ef](https://github.com/glfw/glfw) in Velvet/vendor/GLFW
- [imgui @ 05487c1](https://github.com/jackphillips31/imgui) in Velvet/vendor/imgui
## Installation
Install Velvet with Git Bash or simply download and extract it to your desired directory. If you download the zip, make sure you download and extract the required dependencies to their respective directories.

First, clone the project.
```bash
  git clone https://github.com/jackphillips31/Velvet.git
```
Then update/download the submodules.
```bash
  git submodule init
  git submodule update
```
## Setting up Premake
1. **Download Premake5**
   - Visit the official Premake website to download the appropriate version for your operating system: [Download Premake5](https://premake.github.io/download).
2. **Place Premake5 Executable**
   - Once downloaded, place the `premake5.exe` file into the following directory in your Velvet project:
     ```
     Velvet-main/
     ├── external/
     │   ├── premake/
     │   │   └── bin/
     │   │       └── premake5
     ```
3. **Edit Project Configuration (Optional)**
   - Locate the `Win-GenProjects.bat` script in the `scripts` folder.
   - Open the script with a text editor of your choice.
   - Edit the 3rd line in the script to specify your Visual Studio version or your preferred development environment. For example, change `vs2022` to `vs2017` if that's your version.
   - For other development environments, use the corresponding premake5 command. You can find more information on premake5 usage in the [official documentation](https://premake.github.io/docs/Using-Premake/#using-premake-to-generate-project-files).
   - Save the script after making your changes.
### Generate Project Files
   - Simply run the `Win-GenProjects.bat` script in the `scripts` folder
# Building Velvet
- After generating the project files, open your development environment, load the generated project files, and build the Velvet engine according to your development environment's build instructions.
- _**PS:**_ I only have a Windows machine and have only used Visual Studio 2022. Regarding building with different development environments on Linux or MacOS, I can't say what errors you might encounter. Not to mention I've only created a Window class and Entry Point for Windows.
### Done!
- You've successfully set up Velvet with your preferred development environment and are now ready to start using it for your game development projects.
## Contributing to Velvet
Thank you for your interest in contributing to this Velvet! While it's a small one-person project, I'm committed to making it open source and welcoming any and all contributions. Your input and help are greatly appreciated.
### How to Contribute
1. **Fork the Repository:** Start by forking this repository to your own GitHub account. This will create a copy of Velvet in your account that you can freely make changes to.
2. **Clone the Repository:** Clone the forked repository to your local machine using Git.
   ```bash
   git clone https://github.com/your-username/your-fork.git
   ```
3. **Create a Branch:** Create a new branch for your contribution. This branch will help you isolate your changes.
   ```bash
   git checkout -b feature/your-feature-name
   ```
4. **Make Your Contributions:** Implement your changes, fix bugs, or add new features to Velvet.
5. **Test Your Changes:** Ensure that your changes work as expected and do not introduce any new issues.
6. **Commit Your Changes:** Make sure your fork is up to date then commit your changes with a clear and concise commit message.
   ```bash
   git commit -m "Add your commit message here"
   ```
7. **Push Your Changes:** Push your branch to your forked repository on GitHub
   ```bash
   git push origin feature/your-feature-name
   ```
8. **Create a Pull Request:** Open a pull request from your branch to the main repository. Be sure to provide a detailed description of your changes, the problem you're solving, and any relevant information.
### What Can You Contribute?
You can contribute in various ways, including but not limited to:
- Fixing bugs or issues.
- Adding new features or enhancements.
- Improving documentation.
- Enhancing code quality.
- Providing feedback and suggestions.
### Code of Conduct
Please note that Velvet follows a generally respectful code of conduct. We expect all contributors to adhere to it, ensuring a friendly and inclusive environment.
### Get in Touch
If you have any questions, suggestions, or need assistance, feel free to [open an issue](https://github.com/jackphillips31/Velvet/issues).

Once again, thank you for considering contributing to Velvet. Your involvement is invaluable, and together we can make it even better!
## License
This project is licensed under the MIT License. You can find the full text of the MIT License in the `LICENSE` file in this repository.

The MIT License allows you to freely use, modify, and distribute this software, subject to the terms and conditions of the license. It's a permissive open-source license that encourages collaboration and the creation of derivative works. For more details, please refer to the [MIT License](LICENSE) file.
