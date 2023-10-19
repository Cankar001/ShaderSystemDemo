import os
import subprocess
import platform

print('Your detected System is: ' + platform.system())

# Make sure we are in the root directory
if 'Scripts' in os.getcwd():
    os.chdir('../')
    
print("Running premake...")

if (platform.system() == 'Windows'):
    subprocess.call(["Premake/Windows/premake5.exe", "vs2022"])
elif (platform.system() == 'Linux'):
    subprocess.call(["chmod", "+x", "Premake/Linux/premake5"])
    subprocess.call(["Premake/Linux/premake5", "gmake"])
elif (platform.system() == 'Darwin'):
    subprocess.call(["chmod", "+x", "Premake/MacOS/premake5"])
    subprocess.call(["Premake/MacOS/premake5", "xcode4"])
