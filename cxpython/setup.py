from distutils.core import setup, Extension
setup(name="cxpy", version="1.0",
      ext_modules=[
          Extension("cxpy", ["cxpy_module.cpp","cxpy_noddy2.cpp"]),
          # Extension("noddy2", ["noddy2.c"]),
      ])