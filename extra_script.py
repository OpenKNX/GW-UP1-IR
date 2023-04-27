Import("env")
Import("projenv")
import gzip
import shutil
import re

print("Copying hardware.h")
shutil.copyfile(
    env["PROJECT_DIR"] + '/include/hardware.h',
    env["PROJECT_LIBDEPS_DIR"] + '/' + env["PIOENV"] + '/OGM-Common/include/hardware.h')
print("Copying knxprod.h")
shutil.copyfile(
    env["PROJECT_DIR"] + '/include/knxprod.h',
    env["PROJECT_LIBDEPS_DIR"] + '/' + env["PIOENV"] + '/OGM-Common/include/knxprod.h')

#print(env.Dump())

def post_program_action(source, target, env):
    print("Changing ", source[0].get_path()[0:-4] + ".uf2")

    with open(env["PROJECT_LIBDEPS_DIR"] + '/' + env["PIOENV"] + "/OGM-Common/include/knxprod.h", 'r') as knxprod:
        content = knxprod.read(1000)

    m = re.search("#define MAIN_OpenKnxId 0x([0-9A-Fa-f]{2})", content)
    openknxid = m.group(1)
    m = re.search("#define MAIN_ApplicationNumber 0x([0-9A-Fa-f]{2})", content)
    appnumber = m.group(1)
    m = re.search("#define MAIN_ApplicationVersion 0x([0-9A-Fa-f]{2})", content)
    appversion = m.group(1)
    
    with open(env["PROJECT_SRC_DIR"] + "\\main.cpp", 'r') as knxprod:
        content = knxprod.read(-1)

    m = re.search("firmwareRevision = ([0-9]+);", content)
    apprevision = m.group(1)

    print(openknxid)
    print(appnumber)
    print(appversion)
    print(apprevision)

    with open(source[0].get_path()[0:-4] + ".uf2", "rb") as orig_file:
        barray=bytearray(orig_file.read())
        barray[9] = barray[9] | 0x80
        barray[288] = 8 #Tag Size
        barray[289] = 0x4B #Type
        barray[290] = 0x4E #Type
        barray[291] = 0x58 #Type
        barray[292] = int(openknxid, 16) #Data
        barray[293] = int(appnumber, 16) #Data
        barray[294] = int(appversion, 16) #Data
        barray[295] = int(apprevision) #Data
        with open(source[0].get_path()[0:-4] + "_new.uf2","wb") as file: 
            file.write(barray)

env.AddPostAction("buildprog", post_program_action)