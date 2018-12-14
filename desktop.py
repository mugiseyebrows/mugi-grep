
import os
import shutil

name = 'mugi-grep'

desktop = os.path.expanduser('~/.local/share/applications/%s.desktop' % name)
sizes = ['128x128','16x16','256x256','32x32','48x48','512x512']

icons = os.path.expanduser('~/.local/share/icons/hicolor')
for size in sizes:
    src = os.path.join('./icons',size,'%s.png' % name)
    dst = os.path.join(icons,size,'apps','%s.png' % name)
    shutil.copy(src,dst)

icon = dst = os.path.join(icons,'256x256','apps','%s.png' % name)
executable = '/usr/local/bin/%s' % name
categories = ['Utility']
with open(desktop,'w') as f:
    f.write('[Desktop Entry]\nName=%s\nExec=%s\nIcon=%s\nType=Application\nCategories=%s' % (name,executable,icon,';'.join(categories) + ';'))