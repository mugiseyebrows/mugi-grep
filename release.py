from mugidelivery import Manager

import os

plugins = [p for p in [
'C:\\Qt5\\5.11.1\\mingw53_32\\plugins',
'D:\\qt\\Qt5.11.3\\5.11.3\\mingw53_32\\plugins'
] if os.path.exists(p)]

conf = {
    'AppName' : 'mugi-grep',
    'AppVersion' : '1.0.1',
    'Binaries' : [{'Source': 'D:\\dev\\mugi-grep\\release\\mugi-grep.exe', 'Dest': ''}],

    'BuildDir' : 'D:\\dev\\mugi-grep',
    'Arch': 'win32',
    'VersionUpdater': 'QtVersionUpdater',
    'Packer': 'SimplePacker',
    #'OutputDir': 'D:\\dev\\mugi-grep\\mugi-grep',
    'Data': [{'Source':[
        'D:\\dev\\mugi-grep\\settings.win32.json',
        'D:\\dev\\mugi-grep\\settings.linux.json'
    ]}],
    
    'QtPlugins': ['qwindows','qwindowsvistastyle'],
    'QtPluginsSource': plugins[0]
}

m = Manager(conf)
#m.updateVersion()
#m.build()
m.pack()
#m.release()
