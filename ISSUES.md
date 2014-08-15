# Known Issues ##

## Windows ###

### Error executing: "The application was unable to start correctly (0x000007b)" ###
This is most likely due to a 32-bit/64-bit mismatch between the compiled executable and the `.dll` files it depends on. You can confirm this with a tool like [Dependency Walker](https://en.wikipedia.org/wiki/Dependency_Walker). It may be that the copy of `libsndfile-1.dll` and `openal32.dll` don't match the architecture (bit-wise) of the compiled executable. A remedy is to copy the `.dll` files of the correct architecture to your exectuable's output directory.