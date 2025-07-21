import os
from setuptools import Extension, setup

TT_METAL_HOME = os.getenv("TT_METAL_HOME")
if not TT_METAL_HOME:
    raise RuntimeError("Please set the TT_METAL_HOME environment variable")

setup(
    name="pymetalium",
    version="0.0.1",
    ext_modules=[
        Extension(
            name="pymetalium",
            sources=["pymetalium.cpp"],
            include_dirs=[
                f"{TT_METAL_HOME}/tt_metal/api",
                f"{TT_METAL_HOME}/build_Release/include",
                f"{TT_METAL_HOME}/build_Release/include/metalium-thirdparty",
            ],
            library_dirs=[
                f"{TT_METAL_HOME}/build_Release/lib",
                f"{TT_METAL_HOME}/build_Release/lib64",
            ],
            libraries=[
                "fmt",
                "tt_metal",
            ],
            runtime_library_dirs=[
                f"{TT_METAL_HOME}/build_Release/lib",
                f"{TT_METAL_HOME}/build_Release/lib64",
            ],
            extra_compile_args=[
                "-std=c++20",
            ],
        )
    ],
)
