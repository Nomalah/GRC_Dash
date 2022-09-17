from conans import ConanFile, errors
from conan.tools.cmake import CMake, CMakeToolchain
from conan.tools.layout import basic_layout, cmake_layout
from conan.tools.build import cross_building
from conan.tools.files import copy
from conans.errors import ConanInvalidConfiguration
import os
import glob
import subprocess

class GRCDash(ConanFile):
    scm = {
        "type": "git",
        "url":"auto",
        "revision": "auto"
    }

    name = "GRCDash"
    version = "0.1.0"
    description = "Gryphon Racing Club Dash"
    settings = "os", "compiler", "build_type", "arch"
    
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "dash_year": [2019, 2020, 2022],
        "front_end_only": [True, False]
    }

    default_options = {
        "shared": False,
        "fPIC": True,
        "dash_year": 2019,
        "front_end_only": False
    }

    generators = "CMakeDeps"#, "qt"
    exports_sources = "CMakeLists.txt", "src/*"

    def validate(self):
        if self.settings.os == "Windows" and not self.options.front_end_only:
            raise ConanInvalidConfiguration("Windows backend for canbus not supported")

    def configure(self):
        if self.settings.compiler == 'Visual Studio':
            del self.options.fPIC

    def requirements(self):
        # self.requires("qt/6.3.1")
        self.requires("fmt/9.0.0")
        # self.setOptionsQT()

    def layout(self):
        cmake_layout(self)
    
    def generate(self):
        tc = CMakeToolchain(self)
        #tc.variables["QT_BIN_PATH"] = self.deps_cpp_info["qt"].bin_paths[0].replace("\\", "/")
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()