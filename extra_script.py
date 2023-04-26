Import("env")
import gzip
import shutil

print("Copying hardware.h")
shutil.copyfile('include/hardware.h', '.pio/libdeps/' + env["PIOENV"] + '/OGM-Common/include/hardware.h')
print("Copying knxprod.h")
shutil.copyfile('include/knxprod.h', '.pio/libdeps/' + env["PIOENV"] + '/OGM-Common/include/knxprod.h')

def post_program_action(source, target, env):
    print("Building ", source[0].get_path()[0:-4] + ".uf2")
    #with open(source[0].get_abspath(), 'rb') as orig_file:
    #    with gzip.open(source[0].get_abspath() + ".gz", 'wb') as zipped_file:
    #        zipped_file.writelines(orig_file)

    with open(source[0].get_path()[0:-4] + ".uf2", "rb") as orig_file:
        barray=bytearray(orig_file.read())
        barray[9] = barray[9] | 0x80
        barray[288] = 8 #Tag Size
        barray[289] = 1 #Type
        barray[290] = 1 #Type
        barray[291] = 1 #Type
        barray[292] = 1 #Data
        barray[293] = 2 #Data
        barray[294] = 3 #Data
        barray[295] = 4 #Data
        with open(source[0].get_path()[0:-4] + "_new.uf2","wb") as file: 
            file.write(barray)

env.AddPostAction("buildprog", post_program_action)