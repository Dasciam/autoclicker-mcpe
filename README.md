# Minecraft: Bedrock Edition auto-clicker.
### A slightly customisable hold-auto-clicker.

## Usage
#### To get help, use ./autoclicker_mcpe --help

#### The auto-clicker only works when a window named "Minecraft" is active

## Building
```bash
git clone https://github.com/Dasciam/autoclicker-mcpe.git
git submodule update --init
cd autoclicker-mcpe
mkdir build && cd build
cmake ./../ -DARGS_BUILD_EXAMPLE=OFF -DARGS_BUILD_UNITTESTS=OFF
make
```