from mugidelivery import Manager

conf = {
    'AppName' : 'mugi-grep',
    'AppVersion' : '1.0',
    'Binaries' : [{'Source': 'D:\\dev\\mugi-grep\\release\\mugi-grep.exe', 'Dest': ''}],

    'BuildDir' : 'D:\\dev\\mugi-grep',
    'InfoUrl' : 'https://mugiseyebrows.ru/mugi-grep/',
    
    'Packer': 'SimplePacker',
    'OutputDir': 'D:\\dev\\mugi-grep\\mugi-grep',
    
    'QtPlugins': ['qwindows','qwindowsvistastyle'],
    'QtPluginsSource': 'C:\\Qt5\\5.11.1\\mingw53_32\\plugins'
}

m = Manager(conf)
m.updateVersion()
m.build()
m.pack()
m.release()
