<h3 align="center">Password Manager Tool</h3>

---

## Prerequisites

To successfully building the project, ensure you have the following software installed and configured on your system:

* **CMake**: A cross-platform build system generator for managing the build process C++ project.
* **Vcpkg**: A C++ package manager that simplifies the installation of mongocxx driver.
* **Mongocxx Driver**: The C++ driver for interacting with MongoDB, installed via vcpkg.
* **C++ Compiler (with C++17 Support)**: A C++ compiler capable of compiling code compliant with the C++17 standard. Visual Studio is a recommended option, as it provides comprehensive C++ development tools and supports the C++17 standard out of the box.

## Installing

#### CMake

If CMake is not already installed on your system, follow these step:

1. Navigate the official CMake download page at [https://cmake.org/download](https://cmake.org/download/)
2. Choose the appropriate installer for yout operating system. For Windows with 64-bit architecture, download the [`cmake-3.29.3-windows-x86_64.msi`](https://github.com/Kitware/CMake/releases/download/v3.29.3/cmake-3.29.3-windows-x86_64.msi) file.
3. Run the downloaded installer and follow the default instruction.

#### vcpkg

To install and st up vcpkg, follow these steps: 

1. Open a terminal or command prompt and navigate to your `C:/` directory. Clone the vcpkg repository from Github using the following command:

   ```bash
   cd C:/
   git clone https://github.com/microsoft/vcpkg
   ```
2. Run the bootstrap scripts to prepare vcpkg for use:

   ```
   .\vcpkg\bootstrap-vcpkg.bat
   ```

**Recommendation**: I advised you to install vcpkg in the `C:/`directory to align with the default configuration in the `CMakeLists.txt` file. If you choose a different installation location, you will need to update the `VCPKG_ROOT` variable in your `CMakeLists.txt` accordingly.

#### mongcxx driver

Open Window Powershell and navigate to the vcpkg installation directory (assuming it was installed in `C:/vcpkg`). Ensure you have the lastes version of vcpkg by running `git pull`. Then, install the mongcxx driver.

```bash
cd C:/vcpkg
git pull
./vcpkg install mongo-cxx-driver
```

#### Create enviroment variables

* `MONGOBD-URI` : The connection string of the cluster in mongoDB Atlas of your project. You will need to create the Altas project by yourself. Just use the M0 cluster for free.
* `PM-MASTER-USER` : The master username
* `PM-MASTER-PASSWORD` : The master password

#### Configure and build the project

Clone this repository to your local machine using the following command:

```
git clone https://github.com/HaiTuyen/Password-Manager-CMake.git
```

Navigate the the cloned project directory and create a `build` folder.

```
cd Password-Manager-CMake
mkdir build
cd build
```

Use CMake to generate the build files and then build the project:

```
cmake ..
cmake --build .

```

Upon successfull build, the `pm.exe` will be generated in the `build/Debug` directory. Add this dicrectory to your system PATH environment varibale. This allows you to execute the command without specifying the full path. Restart the computer if neccessary.
