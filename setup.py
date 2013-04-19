from distutils.core import setup, Extension

setup(name="PyRabin", version="0.1", ext_modules=[
    Extension("rabin", [
            "src/pyrabin.c",
            "src/rabin_polynomial.c"
   ])
])
