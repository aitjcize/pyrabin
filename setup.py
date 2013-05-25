from distutils.core import setup, Extension

setup(name="PyRabin", version="0.2", ext_modules=[
    Extension("rabin", [
            "src/pyrabin.c",
            "src/rabin.c",
            "src/rabin_polynomial.c",
            "src/utils.c",
   ], libraries=['crypto'])
])
