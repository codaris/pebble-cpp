# Pebble Foundation Classes (C++ for Pebble)

This Pebble Package gives you the ability to code Pebble watchapps using C++.  It wraps the Pebble API in an low-overhead header-only object-oriented library. 

## Why would you do this?!?

> "The reasonable man adapts himself to the world; the unreasonable one persists in trying to adapt the world to himself. Therefore, all progress depends on the unreasonable man."
\- *George Bernard Shaw*

I mostly work in high-level languages like C#, Java, Python, and C++ and I started to struggle with organizing my Pebble watchapp code in C.  As my projects grew larger, it became harder to juggle all the structs, callback contexts, and especially global variables.  

So in the fall of 2014 I started a project to bring C++ to Pebble development.  There were many false starts and difficult problems.  I eventually got to where all the big problems were resolved and I just needed to build out the library.  But after Pebble closed it's doors I stopped working on it.

When the first [Rebble Hackathon](https://rebble.io/hackathon-001/) was announced, it seemed like the time to try and finish this project or at least release what I have to the world.  

## Features and Benefits

* PFC classes automatically manage the context pointer in Pebble OS callbacks.
* All PFC methods are inline with low-to-zero overhead.
* Ability to use STL classes like `std::string`, `std::vector`, and `std::unique_ptr` for more high-level development.
* Organize your Windows and Layers into their own classes.
* Use RAII to automatically free resources when they are no longer needed.
* Fully compatible with the Pebble C API -- you can call C functions with PFC class instances as parameters and use Pebble C types with C++ classes and methods.  

## Getting Started

To get started, please check out the following pages:

* [Simple C++ Application Tutorial](https://codaris.github.io/pebble-cpp/tutorial.html)
* [Using this SDK](https://codaris.github.io/pebble-cpp/using.html)

They provide detailed information about configuring the project to build C++ files and how to structure your code.

## Documentaton

View the complete documentation by clicking the link below:

* [Pebble Foundation Classes Documentation](https://codaris.github.io/pebble-cpp/)

### Example and Tutorial Programs

You can view and download various sample projects from the site below:

* [Pebble Foundation Classes Example Projects GitHub](https://github.com/codaris/pebble-cpp-examples)

## Getting Help

You can get help with this SDK using the [Issues](https://github.com/codaris/pebble-cpp/issues) tab in this project or by using the [Rebble Alliance Discord](https://discord.com/invite/aRUAYFN).

Also please check out the [Known Issues](https://codaris.github.io/pebble-cpp/issues.html) page.

## Contributing

This project is currently under an active development. Code quality and stability is the primary focus, so any bug fixing requests and/or fixing changes will appreciated. If you want to help this project:

* Any help in testing the SDK are very appreciated. You can leave information about found defects on the [Issues](https://github.com/codaris/pebble-cpp/issues) tab;
* Anyone who would like to provide a fix for any found defect are welcome. Please, create pull requests for fixing changes;
* You are welcome to create pull requests for new features and new Pebble API wrappers;
* Additional [Sample Applications](https://github.com/codaris/pebble-cpp-examples) that demonstrate features of the SDK are appreciated.

#### How to inform about found defects

* First of all, please check project Issues on GitHub and Known Issues in CHANGELOG. The issue you found might be already recorded.
* Check that the issue is reproducible and describe it.
* Ideally, create a small test program that exposes the problem.
