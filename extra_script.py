Import("env")
Import("projenv")
import gzip
import shutil
import re
import os

print("Copying hardware.h")
shutil.copyfile(
    env["PROJECT_DIR"] + '/include/hardware.h',
    env["PROJECT_LIBDEPS_DIR"] + '/' + env["PIOENV"] + '/OGM-Common/include/hardware.h')
print("Copying knxprod.h")
shutil.copyfile(
    env["PROJECT_DIR"] + '/include/knxprod.h',
    env["PROJECT_LIBDEPS_DIR"] + '/' + env["PIOENV"] + '/OGM-Common/include/knxprod.h')

#print(env.Dump())
#print(projenv.Dump())
platform = env.PioPlatform()
print(platform)



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
            

    
            
def check_flash(source, target, env):
    flash_start = 268435456 #0x10000000 got from uf2
    flash_end = flash_start + int(projenv["PICO_FLASH_LENGTH"])
    bin_size = os.stat(source[0].get_path()).st_size
    openknx_start = -1

    for x in projenv["CPPDEFINES"]:
        if type(x) is tuple:
            if x[0] == "KNX_FLASH_OFFSET":
                knx_start = int(x[1], 16)
            if x[0] == "KNX_FLASH_SIZE":
                knx_end = int(x[1], 16)
            if x[0] == "OPENKNX_FLASH_OFFSET":
                openknx_start = int(x[1], 16)
            if x[0] == "OPENKNX_FLASH_SIZE":
                openknx_end = int(x[1], 16)

    knx_start = flash_start + knx_start   
    knx_end = knx_start + knx_end
    if openknx_start != -1:
        openknx_start = flash_start + openknx_start   
        openknx_end = openknx_start + openknx_end

    print("Following bytes are free:")
    print("  Flash -> Parameters     " + str(knx_start - (flash_start + bin_size)))   

    file_start = -1
    file_start = int(env["FS_START"]) #0x1017F000 -> 270004224
    file_end = int(env["FS_END"])     #0x101FF000 -> 270528512

    if openknx_start != -1:
        print("  Parameters -> OpenKnx   " + str(openknx_start - knx_end))
        print("  OpenKNX -> Flash End    " + str(flash_end - openknx_end))
    else:
        print("  Parameters -> Flash End  " + str(flash_end - knx_end))


    print("knx start  " + str(knx_start))
    print("knx end    " + str(knx_end))
    print("oknx start " + str(openknx_start))
            


env.AddPostAction("buildprog", post_program_action)

if projenv["BOARD_MCU"] == "rp2040":
    env.AddPostAction("buildprog", check_flash)