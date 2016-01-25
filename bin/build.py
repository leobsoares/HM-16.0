import os
import sys

AppList = ['nosatd4/', 'app_zeros_1_4_4/', 'app_zeros_2_4_4/', 'app_zeros_3_4_4/', 'app_zeros_4_4_4/', 'app_zeros_5_4_4/', 'app_zeros_6_4_4/', 'app_zeros_7_4_4/', 'app_zeros_8_4_4/', 'app_zeros_9_4_4/', 'app_zeros_10_4_4/', 'app_zeros_11_4_4/', 'app_zeros_12_4_4/', 'app_zeros_13_4_4/', 'app_zeros_14_4_4/', 'app_zeros_15_4_4/']

count = 1

for app in AppList:

    if not os.path.exists(app):
        os.makedirs(app)

    fd = open('copy.h', 'r')
    fd1 = open('../source/Lib/TLibCommon/AppComp.h', 'w')

    if app == 'nosatd4/':
        for l in fd.readlines():
            if 'NOSATD4' in l:
                fd1.write('#define NOSATD4 1\n')
            elif 'APPSATD4' in l:
                fd1.write('#define APPSATD4 0\n')
            else:
                fd1.write(l)
        fd.close()
        fd1.close()
    else:
        for l in fd.readlines():
            if 'APPSATD4' in l:
                fd1.write('#define APPSATD4 8')
            elif 'KSATD4' in l:
                fd1.write('#define KSATD4 '+str(count)+'\n')
            else:
                fd1.write(l)
        fd.close()
        fd1.close()
        os.system('touch '+app+'app_zeros'+str(count))
        count = count + 1


    os.chdir('../build/linux')
    os.system('make clean')
    os.system('make -j8')
    os.chdir('../../bin')
    os.system('mv annex* convert* TAppDecoder* TAppEncoder* '+app)

if not os.path.exists('correct/'):
    os.makedirs('correct/')

if not os.path.exists('nohad/'):
    os.makedirs('nohad/')

os.system('cp copy.h ../source/Lib/TLibCommon/AppComp.h')
os.chdir('../build/linux')
os.system('make clean')
os.system('make -j8')
os.chdir('../../bin')
os.system('cp annex* convert* TAppDecoder* TAppEncoder* correct/')
os.system('cp annex* convert* TAppDecoder* TAppEncoder* nohad/')
