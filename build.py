#!/usr/bin/env python3
import subprocess
import sys
import os
from pathlib import Path

class Builder:
    def __init__(self):
        self.platform = sys.platform
        self.compiler = "clang++"
        self.flags = ["-std=c++17", "-O2", "-g"]
        #self.imgui_dir = "ThirdParty/imgui"
    
    def build(self):
        sources = self.find_sources()
        includes = self.get_includes()
        libraries = self.get_libraries()
        
        cmd = (
            [self.compiler]
            + self.flags
            + includes
            + sources
            + libraries
            + ["-o", "application"]
        )
        
        if self.platform == "win32":
            cmd.extend(["-lSDL3", "-limm32", "-loleaut32"])
        else:
            cmd.extend(["-lSDL3", "-ldl", "-lpthread"])
        
        subprocess.run(cmd)

    def find_sources(self):
        src = []
        for root, dirs, files in os.walk("."):
            for file in files:
                if file.endswith(".cpp"):
                    src.append(os.path.join(root, file)) 
        return src

    def get_includes(self):
        return [
            r"-IC:\msys64\mingw64\include",
            "-I.",
            #"-I" + self.imgui_dir,
            #"-I" + self.imgui_dir + "/backends",
            #"-I" + self.imgui_dir + "/backends/vu"
        ]
    
    def get_libraries(self):
        return [
            r"-LC:\msys64\mingw64\lib",
            "-L."
        ]

    def run(self):
        subprocess.run(["./application"])
        
if __name__ == "__main__":
    build = Builder()
    if sys.argv[1] == "run":
        build.run()
    else:
        build.build()