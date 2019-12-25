# The Duck2200 Assembler


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
* register keyword removed

Experience with new CMake 3.16 features
* Precompiled headers added to cmake, target_precompile_headers - awesome!
* Unity builds also added to cmake, CMAKE_UNITY_BUILD=ON

Redesigning this thing as a library is hard
* With my experience from work I wanted to try to redesign this project into a library with Emulator and Assembler executables on top.
    While I can picture what it would look like, breaking it off into a library with two executables (or even one) seems pretty pointless.