# WEB-SERVER

A simple, multi-platform web server written in C, designed to serve static files over HTTP.

## Features

* Handles HTTP GET and HEAD requests.
* Serves static files from a configurable web root directory.
* Includes a custom logging system with INFO, DEBUG, WARNING, and ERROR levels.
* Supports command-line argument parsing for server configuration (interface, port, web root).
* Basic cross-platform compatibility (Windows and Unix-like systems).

## Building the Project

This project uses CMake for building.

### Prerequisites

* CMake 3.30 or higher
* A C compiler (e.g., GCC, Clang, MSVC)

### Steps

1.  **Clone the repository:**
    ```bash
    git clone <repository_url>
    cd web-server
    ```

2.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Configure CMake:**
    ```bash
    cmake ..
    ```
    * On Windows, if you have multiple generators installed, you might need to specify one, e.g.:
        ```bash
        cmake .. -G "MinGW Makefiles"
        ```
      or
        ```bash
        cmake .. -G "Visual Studio 17 2022"
        ```

4.  **Build the project:**
    ```bash
    cmake --build .
    ```

This will generate the `web-server` executable in your `build` directory.

## Usage

The server can be run with various command-line arguments:

```bash
./web-server [options] <ip_address> <port> <web_root_directory>
