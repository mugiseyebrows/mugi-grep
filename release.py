from mugidelivery import Manager

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
    'QtPluginsSource': 'C:\\Qt5\\5.11.1\\mingw53_32\\plugins'
}

m = Manager(conf)
#m.updateVersion()
#m.build()
m.pack()
#m.release()
