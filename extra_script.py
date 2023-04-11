Import("env")
import gzip
import shutil

print("Copying hardware.h")
shutil.copyfile('include/hardware.h', '.pio/libdeps/' + env["PIOENV"] + '/OGM-Common/include/hardware.h')
print("Copying knxprod.h")
shutil.copyfile('include/knxprod.h', '.pio/libdeps/' + env["PIOENV"] + '/OGM-Common/include/knxprod.h')

def post_program_action(source, target, env):
    print("Building ", source[0].get_path() + ".gz")
    with open(source[0].get_abspath(), 'rb') as orig_file:
        with gzip.open(source[0].get_abspath() + ".gz", 'wb') as zipped_file:
            zipped_file.writelines(orig_file)

env.AddPostAction("buildprog", post_program_action)