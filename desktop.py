
import subprocess
import os

name = 'mugi-grep'

ico = './mugi-grep.ico[1]'
desktop = '/home/overloop/.local/share/applications/%s.desktop' % name
sizes = ['128x128','16x16','256x256','32x32','48x48','512x512']

dst = os.path.expanduser('~/.local/share/icons/hicolor')
for size in sizes:
    cmd = ['convert',ico,'-resize',size,os.path.join(dst,size,'apps','%s.png' % name)]
    subprocess.check_output(cmd)

icon = os.path.join(dst,'512x512','apps','%s.png' % name)
executable = '/usr/local/bin/mugi-grep'
categories = ['Utility']
with open(desktop,'w') as f:
    f.write('[Desktop Entry]\nName=%s\nExec=%s\nIcon=%s\nType=Application\nCategories=%s' % (name,executable,icon,';'.join(categories) + ';'))