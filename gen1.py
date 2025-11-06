def getPubName(privName):
    return privName[1].lower() + privName[2:]

def setName(privName):
    pubName = getPubName(privName)
    return "set" + pubName[0].upper() + pubName[1:]

def fnArg(type, name):
    if type in ['int','double'] or '::' in type:
        return f"{type} {name}"
    return f"const {type}& {name}"

def exprGetter1(className, type, privName):
    pubName = getPubName(privName)
    return f"{type} {pubName}() const {{\nreturn {privName};\n}}"

def exprSetter1(className, type, privName):
    pubName = getPubName(privName)
    return f"{className}& {pubName}({fnArg(type, 'value')}) {{\n{privName} = value;\nreturn *this;\n}}"

def exprSetter2(className, type, privName):
    return f"void {setName(privName)}({fnArg(type, 'value')}) {{\n{privName} = value;\n}}"

def exprCtor(className, members):
    args = []
    init = []
    for member in members:
        if not member.endswith(';'):
            continue
        member = member.rstrip(';')
        type, privName = member.split(' ', 1)
        pubName = getPubName(privName)
        args.append(fnArg(type, pubName))
        init.append(f'{privName}({pubName})')
    return f'{className}({", ".join(args)}) : {", ".join(init)} {{\n\n}}'

def exprGetter1(className, type, privName):
    pubName = getPubName(privName)
    return f"{type} {pubName}() const {{\nreturn {privName};\n}}"

def exprSetter1(className, type, privName):
    pubName = getPubName(privName)
    return f"{className}& {pubName}({fnArg(type, 'value')}) {{\n{privName} = value;\nreturn *this;\n}}"

def exprSetter2(className, type, privName):
    return f"void {setName(privName)}({fnArg(type, 'value')}) {{\n{privName} = value;\n}}"

def gen(className, members):

    getters = []
    setters = []
    ctors = []

    for member in members:
        if not member.endswith(';'):
            continue
        member = member.rstrip(';')
        type, privName = member.split(' ', 1)
        getters.append(exprGetter1(className, type, privName))
        setters.append(exprSetter2(className, type, privName))

    ctors.append(exprCtor(className, members))

    with open(f"D:\\w\\{className.lower()}.cpp", "w", encoding='utf-8') as f:
        for item in [['public:'], ctors, getters, setters, ['private:'], members]:
            print("\n".join(item) + "\n", file=f)

def main():
    className = "SearchHitsWithContext"
    members = """RegExpPair mPattern;
QList<SearchHitWithContext> mHits;
int mTotal;
int mComplete;
int mFiltered;
QString mLast;
""".split("\n")
    gen(className, members)


if __name__ == "__main__":
    main()