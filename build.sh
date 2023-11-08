mkdir -p build
cd build
: ${OPTIONS:=""}
cmake $OPTIONS \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_INSTALL_PREFIX=$HOME/opt/x-disk ..
make -j4
cd ..
