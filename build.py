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
        self.imgui_dir = "ThirdParty/imgui"
        self.sources = self.find_sources()
        self.includes = self.get_includes()
        self.libraries = self.get_libraries()
        self.objfiles = []
        self.app_name = "Build/SimulateurPlanetaire3D"
    
    def build(self):
        self.build_manager()
        self.objfiles = self.getObjetfile()
        cmd = (
            [self.compiler]
            + self.flags
            + self.includes
            + self.objfiles
            + self.libraries
            + ["-o", self.app_name]
        )
        
        if self.platform == "win32":
            cmd.extend(["-lSDL3", "-limm32", "-loleaut32", "-mconsole"])
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
            "-I" + self.imgui_dir,
            "-I" + self.imgui_dir + "/backends"
        ]
    
    def get_libraries(self):
        return [
            r"-LC:\msys64\mingw64\lib",
            "-L."
        ]
    
    def build_manager(self):
        obj_dir = os.path.join("Build", "obj")
        if not os.path.exists(obj_dir):
            os.makedirs(obj_dir)

        dll_file = [file for file in os.listdir(".") if file.endswith(".dll")]

        for file in dll_file:
            subprocess.run(["copy", file, "Build"], shell=True)

        for file in self.sources:
            arborescence = os.path.splitext(file)[0]
            nom_pur = arborescence.split(os.sep)[-1]
            obj_path = os.path.join(obj_dir, nom_pur + ".o")

            recompiler = True
            if os.path.exists(obj_path):
                obj_time = os.path.getmtime(obj_path)
                cpp_time = os.path.getmtime(file)
                
                if cpp_time < obj_time:
                    recompiler = False

            if recompiler:
                print(f"Compilation de {file}...")
                commande = [self.compiler, "-c", file, "-o", obj_path]
                subprocess.run(commande)

    def getObjetfile(self):
        objfiles = []
        for root, dirs, files in os.walk("Build/obj") :
            for file in files :
                objfiles.append(os.path.join(root, file))
        
        return objfiles

    def run(self):
        subprocess.run(["./"+ self.app_name])

    def clear(self):
        build_path = Path("Build")
        if build_path.exists() and build_path.is_dir():
            for item in build_path.iterdir():
                if item.is_file():
                    item.unlink()
                elif item.is_dir():
                    for subitem in item.iterdir():
                        subitem.unlink()
                    item.rmdir()
            build_path.rmdir()
        
if __name__ == "__main__":
    build = Builder()
    if sys.argv[1] == "run":
        build.run()
    elif sys.argv[1] == "build":
        build.build()
    elif sys.argv[1] == "clear":
        build.clear()