import os
import sys
import re

VideoList = ['BlowingBubbles']
AppList = ['nosatd4/', 'app_zeros_1_4_4/', 'app_zeros_2_4_4/', 'app_zeros_3_4_4/', 'app_zeros_4_4_4/', 'app_zeros_5_4_4/', 'app_zeros_6_4_4/', 'app_zeros_7_4_4/', 'app_zeros_8_4_4/', 'app_zeros_9_4_4/', 'app_zeros_10_4_4/', 'app_zeros_11_4_4/', 'app_zeros_12_4_4/', 'app_zeros_13_4_4/', 'app_zeros_14_4_4/', 'app_zeros_15_4_4/']
QPList = ['22', '27', '32', '37']
NFrames = sys.argv[1]
cfg = sys.argv[2]

if cfg == 'low':
    cfg = 'encoder_lowdelay_main.cfg'
elif cfg == 'random':
    cfg = 'encoder_randomaccess_main.cfg'
else:
    print('unknown cfg')


for video in VideoList:
    for app in AppList:
        fd1 = open(app+'result_'+video+'.txt', 'w')
        for qp in QPList:

            os.system('./'+app+'TAppEncoderStatic -c ../cfg/'+cfg+' -c ../cfg/per-sequence/'+video+'.cfg --QP='+qp+' -f '+NFrames+' >> '+app+'report_'+video+'_'+qp+'.txt')

            fd = open(app+'report_'+video+'_'+qp+'.txt', 'r')

            l = fd.readline()

            while 'SUMMARY' not in l:
                l = fd.readline()

            l = fd.readline()
            l = fd.readline()

            infos = re.match(r'\s*\d*\s*a\s*(\d*.\d*)\s*(\d*.\d*).*',l,0)

            fd1.write(qp+','+infos.group(1)+','+infos.group(2)+'\n')

        fd.close()
        fd1.close()



for video in VideoList:
    fd1 = open('correct/result_'+video+'.txt', 'w')
    for qp in QPList:

        os.system('./TAppEncoderStatic -c ../cfg/'+cfg+' -c ../cfg/per-sequence/'+video+'.cfg --QP='+qp+' -f '+NFrames+' >> correct/report_'+video+'_'+qp+'.txt')

        fd = open('correct/report_'+video+'_'+qp+'.txt', 'r')

        l = fd.readline()

        while 'SUMMARY' not in l:
            l = fd.readline()

        l = fd.readline()
        l = fd.readline()

        infos = re.match(r'\s*\d*\s*a\s*(\d*.\d*)\s*(\d*.\d*).*',l,0)

        fd1.write(qp+','+infos.group(1)+','+infos.group(2)+'\n')
    fd.close()
    fd1.close()

for video in VideoList:
    fd1 = open('nohad/result_'+video+'_without_SATD.txt', 'w')
    for qp in QPList:

        os.system('./TAppEncoderStatic -c ../cfg/'+cfg+' -c ../cfg/per-sequence/'+video+'.cfg --QP='+qp+' -f '+NFrames+' --HadamardME=0 >> nohad/report_'+video+'_'+qp+'.txt')

        fd = open('nohad/report_'+video+'_'+qp+'.txt', 'r')

        l = fd.readline()

        while 'SUMMARY' not in l:
            l = fd.readline()

        l = fd.readline()
        l = fd.readline()

        infos = re.match(r'\s*\d*\s*a\s*(\d*.\d*)\s*(\d*.\d*).*',l,0)

        fd1.write(qp+','+infos.group(1)+','+infos.group(2)+'\n')
    fd.close()
    fd1.close()


