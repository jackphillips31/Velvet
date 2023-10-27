# Velvet - A Physics/Game Engine

Velvet is an open-source physics and game engine written in C++. It's designed to be beginner-friendly, making application development with Velvet easy and enjoyable.
## Table of Contents
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Generate Project Files](#generate-project-files)
- [Building Velvet](#building-velvet)
- [License](#license)
## Dependencies
- [spdlog](https://github.com/gabime/spdlog) in Velvet/vendor/spdlog
## Installation
Install Velvet with Git Bash or simply download and extract to your desired directory. If you download the zip, make sure you download and extract the required dependencies to their respective directories.

First clone the project.
```bash
  git clone https://github.com/jackphillips31/Velvet.git
```
Then update the submodules.
```bash
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
### Note on the First Build
- Your first build might fail because the directory that the post-build command copies your shared Velvet library to doesn't exist yet. However, after the that first failure, this command should work perfectly, and your project should build if it has been set up right.
- **PS:** I only have a Windows machine and have only used Visual Studio 2022. As far as building with different development enviroments on Linux or MacOS, I can't say what errors you might encounter.
### Done!
- You've successfully set up Velvet with your preferred development environment and are now ready to start using it for your game development projects.
## Contributing
Since Velvet is in its early stages, contributions are not accepted at the moment. However, stay tuned for updates on how you can get involved in the project as it progresses.
## License
This project is licensed under the MIT License. You can find the full text of the MIT License in the `LICENSE` file in this repository.

The MIT License allows you to freely use, modify, and distribute this software, subject to the terms and conditions of the license. It's a permissive open-source license that encourages collaboration and the creation of derivative works. For more details, please refer to the [MIT License](LICENSE) file.
