import os
import subprocess
import platform

system = platform.system()
print('Your detected System is: ' + system)

# Make sure we are in the root directory
if 'Scripts' in os.getcwd():
    os.chdir('../')
    
print("Running premake...")

if (system == 'Windows'): # generate Visual Studio solution for Windows
    subprocess.call(["Premake/Windows/premake5.exe", "vs2022"])
elif (system == 'Linux'): # generate Makefiles for Linux
    subprocess.call(["chmod", "+x", "Premake/Linux/premake5"])
    subprocess.call(["Premake/Linux/premake5", "gmake"])
elif (system == 'Darwin'): # generate XCode solution for MacOS
    subprocess.call(["chmod", "+x", "Premake/MacOS/premake5"])
    subprocess.call(["Premake/MacOS/premake5", "xcode4"])
