from distutils.core import setup, Extension
setup(name="cxpy", version="1.0",
      ext_modules=[
          Extension("noddy", ["noddy_module.cpp","noddy_config.hpp","noddy1.cpp","noddy2.cpp","noddy3.cpp","noddy4.cpp"]),
          Extension("shoddy", ["shoddy_module.cpp","shoddy_config.hpp","shoddy1.cpp"]),
      ])