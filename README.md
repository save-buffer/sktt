# SKTT: Sasha's Kernels for Tenstorrent
SKTT (pronounced "skit" or "skeet") is a collection of kernels written for Tenstorrent accelerators. This has the
benefit of providing a minimal location for your devloop while developing kernels.

## Usage
Make sure you've [installed TT-Metalium](https://docs.tenstorrent.com/tt-metal/latest/tt-metalium/installing.html),
and have your `TT_METAL_HOME` environment variable set to where you cloned the `tt-metalium` directory. You should
not need any additional dependencies as the `build.sh` script takes care to reuse the dependencies installed by
TT-Metalium.

After installing TT-Metalium and setting the env var, you should just be able to run `build.sh`. This will invoke
`g++` on the kernels here, as well as run them. Feel free to modify the script during your devloop to make
building and running the kernel you're working on easier.

