from distutils.core import setup, Extension
setup(name="noddy", version="1.0",
      ext_modules=[
          Extension("noddy", ["noddy_module.cpp","noddy_config.cpp","noddy1.cpp","noddy2.cpp","noddy3.cpp","noddy4.cpp"]),
      ])