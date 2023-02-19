# import required module
import os
import sys

# assign directory
try:
    directory=os.path.dirname(os.path.realpath(sys.argv[1]))
    print(directory)
except:
    print('Please pass directory')
# iterate over files in
# that directory
for filename in os.listdir(directory):
    if(filename != "ResizeAll_script.py"):
        resize = "convert " + filename + " -resize 50% " + filename + "_RESIZED.png"
        os.system('cmd /c ' + resize)
print("Complete!")