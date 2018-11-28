
import os
import re

root_ = '.'

headers = dict()

for root, dirs, files in os.walk('.'):
    for f in [f for f in files if os.path.splitext(f)[1] in ['.h'] ]:
        p = os.path.join(root,f)
        x = os.path.relpath(p,root_).replace('\\','/')
        headers[f] = x

#print(headers)

for root, dirs, files in os.walk('.'):
    for f in [f for f in files if os.path.splitext(f)[1] in ['.cpp','.h'] ]:
        p = os.path.join(root,f)
        with open(p,encoding='utf-8') as x:
            lines = x.readlines()
        changed = False
        for i,line in enumerate(lines):
            m = re.match('#include "(.*)"',line)
            if (m):
                b = os.path.basename(m.group(1))
                if b in headers:
                    if headers[b] != b:
                        line_ = line.replace(m.group(1),headers[b])
                        lines[i] = line_
                        changed = True
        if changed:
            print('writing',p)
            with open(p,'wb') as x:
                x.write(''.join(lines).encode('utf-8'))
                        

            