from distutils.core import setup, Extension
setup(name="cxpy", version="1.0",
      ext_modules=[
          Extension("noddy", ["noddy/noddy_module.cpp","noddy/noddy_config.hpp","noddy/noddy1.cpp","noddy/noddy2.cpp","noddy/noddy3.cpp","noddy/noddy4.cpp"]),
          Extension("shoddy", ["shoddy/shoddy_module.cpp","shoddy/shoddy_config.hpp","shoddy/shoddy1.cpp"]),
      ])