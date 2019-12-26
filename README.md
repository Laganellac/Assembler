# The Duck2200 Assembler

## Why

I thought it would be fun.

I originally did this project a couple years ago for my Software Design class. 
Reading through Dr. Miller's website I can see the specifications and name of the assembler have changed since I completed it.
This project seemed small enough for me to work on in my free time over the holidays and was a good place for me to play around with some new CMake and C++ features.

Originally I wanted to redesign the project into a library that had two executables sitting ontop of it - the assembler and emulator. 
The assembler would spit out a binary file that represented the memory of the program that the emulator would then read.
Both executables would link to the same core library that knew how to read/write the instructions as ascii and program memory as binary.
Redesigning in this way would be both easy and hard.
**Easy** because it's just a matter of taking the code I have now and defining some new CMake targets.
**Hard** because it would feel pretty pointless. The library could expose the Assembler and Emulator classes and the driver programs could call upon that code.
Which would work I just didn't see the point for such a small project. 

## Features

The program does everything it's supposed todo as outlined [here](https://phobos.ramapo.edu/~vmiller/SoftwareDesign/AssemProjectBoardNotes.htm).
Obviously I'm not turning this in for a grade this time around so there's no documentation nor did I follow Dr. Miller's coding standard.

I also changed up some of the design
* Removed the *using namespace std* from the header that got included by every file. Definitely not a good habit to be teaching people, writing std:: really ins't that ugly or annoying. If people want to put *using namespace std* in their implementation then fine but never expose it in a header file (although it was in the pch, is that better or worse?).
* Not everything is a class
    * Errors -> Error namespace. Errors.h declared a class that had all static members. This isn't Java or C#, we don't need todo something like that.
    * Instruction -> Instruction struct in Instruction namespace. Instruction was being used in a really weird way in the provided design skeleton (in PassI). m_instr was never used outside of PassI and should've been destructed and reconstructed on every iteration of the loop. I probably could've (should've?) left it as a class but why do that when its only member function essentially constructs it.
* Added Duck namespace
* Renamed the executable to dasm
* Created Duck2200Constants header. A utility header so that it's easy to update a constant or change an opcode representation. As the file grew bigger I realized I probably should split these back up into their respective header/namespaces. Will probably update this.

## Future features

* Remove the FileAccess class
    * Something like this would make sense if the standard library's fstream wasn't already cross platform.
* Setup CMake so that *make install* works
    * Also get some experience making a project CPack compatible (if it isnt already)
* Add command line arguments to toggle some of the warnings
* Have it serialize the Emulator's memory out as binary so that it could be loaded by a standalone emulator (maybe)
* Turn it into a library (maybe)

## Building

I've tested that it builds on both Linux and Windows. You just need to install CMake (3.16+ preferred) if you don't already have it (and obviously a compiler/build system).

```bash
# You've already cloned the repo
cd Assembler
mkdir build
cd build
cmake ..
make # (or ninja or open up the VisualStudio .sln, whatever build system)
# The executable will be under Assembler/build/bin
```

## What'd I learn

Importance of *static* keyword outside of classes
* When used on functions/variables outside of classes *static* means internal linkage only. 
    I've heard of anonymous namespaces being used for this, but they break unity builds as far as I know.
* Apparently this keyword was intended on being deprecated but luckily it's been kept in the standard.
    Personally I'm not entirely sure why the standard would force people to use anonymous namespaces to achieve internal linkage when the static keyword exists.
* For example the *static* keyword should be used on fuctions/variables inside implementation files that won't be used outside the file.
* All functions defined in a namespace or the "global namespace" have external linkage by default. Same goes for variables (I think) unless they are const (probably should double check on this factoid).

Experience with some of the C++17 features
* Used std::variant in my Instruction struct
* Used commas in *using* statements
* *register* keyword removed (not that I ever really used it)

Experience with new CMake 3.16 features
* Precompiled headers added to cmake, target_precompile_headers - awesome!
* Unity builds also added to cmake, CMAKE_UNITY_BUILD=ON
    * Learned about some of the limitations of Unity builds - it's literally including a bunch of implementation files into one. 

Redesigning this thing as a library is hard
* With my experience from work I wanted to try to redesign this project into a library with Emulator and Assembler executables on top.
    While I can picture what it would look like, breaking it off into a library with two executables (or even one) seems pretty pointless.