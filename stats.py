if __name__ != "__main__":
    exit()

import os
import time

numOfLines = 0

for filename in os.listdir("."):
    if not os.path.isfile(filename):
        continue
    
    extension = filename.split(".")[-1]
    if extension != "cpp" and extension != "h":
        continue
    
    fileObject = open(filename)
    numOfLines += len(fileObject.readlines())
    fileObject.close()

debugExecutablePath = "./Debug/egyptian_jumper"

print("------ Project stats ------")
print("Number of lines: " + str(numOfLines))
print("Last compilation (Debug): " +
      time.strftime("%Y-%m-%d %H:%M:%S",
                    time.localtime(os.path.getmtime(debugExecutablePath))))
print("Executable size (Debug): " +
      str(os.path.getsize(debugExecutablePath)/1024/1024)+" MB")
