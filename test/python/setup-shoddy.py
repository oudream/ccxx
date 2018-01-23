from distutils.core import setup, Extension
setup(name="shoddy", version="1.0",
      ext_modules=[
          Extension("shoddy", ["shoddy_module.cpp","shoddy_config.hpp","shoddy1.cpp"]),
      ])