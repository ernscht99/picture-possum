# Picture Possum
A program for manually, but efficiently, tagging and sorting large numbers of images.
![Screenshot of the UI](res/ui.png)
Features:
 - duplicate detection
 - creation date inferrence
 - custom tags
 - keyboad shortcuts to quickly tag images
 - json export
 - creating a symlink based index structure in your filesystem according to your tags and creation dates
## Development
### Requirements
(Not tested for lower versions, so they may still work)
cmake 3.28.3
g++13
OpenSSL 3.0.13
GTest 1.14.0
Qt 6.4.2

### Ubuntu 24
```
sudo apt install -y cmake g++ libssl-dev qt6-base-dev libgtest-dev build-essential 
```

### Build
```
mkdir build
cmake -B build
cd build
make
```

### Making .deb releases
Build first, install dpdk, then use 
```
chmod +x makerelease.sh
./makerelease.sh version_number release_number
```

# Misc
Made with [TinyEXIF](https://github.com/cdcseacave/TinyEXIF)
