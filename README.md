<picture>
 <source media="(prefers-color-scheme: dark)" srcset="/Resources/images/Velvet-Banner-S.png">
 <source media="(prefers-color-scheme: light)" srcset="/Resources/images/Velvet-Banner-S-LM.png">
 <img alt="Velvet Logo" src="/Resources/images/Velvet-Banner-S.png">
</picture>

Velvet is an open-source physics and game engine written in C++. It's designed to be beginner-friendly, making application development with Velvet easy and enjoyable.
## Table of Contents
- [Current State](#the-state-of-velvet)
- [What is being worked on now](#next-step)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Generate Project Files](#generate-project-files)
- [Building Velvet](#building-velvet)
- [Contributing](#contributing-to-velvet)
- [License](#license)
# The State of Velvet
<picture>
  <source media="(prefers-color-scheme: dark)" srcset="/Resources/images/Current-State-Banner.png">
  <source media="(prefers-color-scheme: light)" srcset="/Resources/images/Current-State-Banner-LM.png">
  <img alt="An image showing the window created when building and running Velvet" src="/Resources/images/Current-State-Banner.png">
</picture>

[Click here for full photo](/Resources/images/Current-State-1.png)
### What does this show?
Presently, we can draw primitives to the screen. What isn't so obvious is the abstraction that is taking place behind the scenes. Using Modern OpenGL it isn't so hard to do what is shown, but I have abstracted away the OpenGL calls. For example, this is how the red quad in the middle of the screen was drawn...

[Sandbox2D.cpp](/Window/src/Sandbox2D.cpp)
```c++
Velvet::Renderer2D::BeginScene(m_CameraController.GetCamera());
Velvet::Renderer2D::DrawQuad(
	{ 0.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.8f, 0.2f, 0.3f, 1.0f }
);
Velvet::Renderer2D::EndScene();
```
The first two arguments being position and size (which haven't been implemented yet) and the third argument is color. Thanks to the abstraction, adding functionality to the first two arguments will be relatively simple.
### What isn't so easy to see?
A lot of the functionality isn't apparent when looking at that screenshot of the current build. There is a multitude of functionalities so far including but not limited to:
- Input Polling
- Event handling
- Dear ImGui integration
- Projection Matrix scaling on Window Resize
- Camera movement (translation, rotation, and zoom)
- Pixel Coordinates to Normalized Device Coordinates conversion
- and more!
### What is currently being worked on?
I am currently working on the 2D renderer. Specifically, attempting to implement a UI system for the 2D renderer. The quads being rendered in the corners of the windows are the beginning of this work. Currently, they stay in place and are scaled correctly when the window is resized. (They maintain the same pixel dimensions now matter what the window's size is)
### Next Step
The current goal is to implement user interaction with the UI elements. Since the element's vertices use Normalized Device Coordinates I will have to convert those to pixel coordinates accounting for scale that way I can use the converted values to compare with the mouse's position. Currently, the Sandbox2D layer has its own ```UICamera``` that it passes on to the ```RendererUI```. The ```RendererUI``` is a static object and although the camera is passed in the ```BeginScene``` function, the draw functions don't have access to it. Because of this, I am thinking of implementing a ```UIController``` that will be instantiated in the Sandbox2D Layer. This ```UIController``` would have ownership over ```RendererUI``` and the ```UICamera```. The box on the bottom-left of the screen is currently drawn using this code:

[Sandbox2D.cpp](/Window/src/Sandbox2D.cpp)
```c++
Velvet::RendererUI::BeginScene(m_UICameraController.GetCamera());
Velvet::RendererUI::DrawButton(
	{ 0.0f, 0.0f },
	{ 0.8f, 1.0f },
	{ 0.2f, 0.8f, 0.2f, 1.0f },
	Velvet::RendererUI::Orientation::BottomLeft
);
Velvet::RendererUI::EndScene();
```
With the ```UIController``` it would look something like this:
```c++
// Instantiating it
Scope<Velvet::UIController> m_UIController = CreateScope<Velvet::UIController>();

// Using it
m_UIController.AddButton(
	{ 0.0f, 0.0f },
	{ 0.8f, 1.0f },
	{ 0.2f, 0.8f, 0.2f, 1.0f },
	Velvet::RendererUI::Orientation::BottomLeft
);
```
## Dependencies
- [spdlog @ 5e11d94](https://github.com/jackphillips31/spdlog/tree/5e11d945a08d7497aee195d643a4d5e4f61a4410) in Velvet/vendor/spdlog
- [GLFW @ ad65e22](https://github.com/jackphillips31/glfw/tree/ad65e224f20e6aba4730046a1b3003b12192ed8f) in Velvet/vendor/GLFW
- [imgui @ ff0eae3](https://github.com/jackphillips31/imgui/tree/ff0eae33663920b41d89afab59b22de592c235e1) in Velvet/vendor/imgui
- [glm @ 586a402](https://github.com/g-truc/glm/tree/586a402397dd35d66d7a079049856d1e2cbab300) in Velvet/vendor/glm
- [GLAD gl 4.6](https://glad.dav1d.de) in Velvet/vendor/GLAD
- [stb_image](https://github.com/nothings/stb/tree/master) in Velvet/vendor/stb_image
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
