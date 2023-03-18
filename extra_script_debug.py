Import("env")
import shutil

print("Copying hardware.h")
shutil.copyfile('include/hardware.h', '.pio/libdeps/debug/OGM-Common/include/hardware.h')
print("Copying knxprod.h")
shutil.copyfile('include/knxprod.h', '.pio/libdeps/debug/OGM-Common/include/knxprod.h')