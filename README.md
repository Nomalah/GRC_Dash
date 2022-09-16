# GRC Dash

## About

An electronic dash for the different GRC vehicles 

## Installing & Building with Conan
Run the conan install script (`-pr` is the profile being used, the options are found in [ConanProfiles](./ConanProfiles/))
```bash
conan install . -if build -pr:h=ConanProfiles/Visual-Studio-2019-Release -pr:b=ConanProfiles/Visual-Studio-2019-Release --build=missing
```

Run the conan build script
```bash
conan build -if build .
```

Final executables will be found in the build folder.