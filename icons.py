import os
import subprocess

name = 'mugi-grep'
sizes = ['128x128','16x16','256x256','32x32','48x48','512x512']
ico = './mugi-grep.ico[1]'

for size in sizes:
    d = os.path.join('./icons',size)
    if not os.path.exists(d):
        os.mkdir(d)
    dst = os.path.join('./icons',size,'%s.png' % name)
    cmd = ['convert',ico,'-resize',size,dst]
    subprocess.check_output(cmd)