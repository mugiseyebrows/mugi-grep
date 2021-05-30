#include "parsepython.h"

#include <QRegularExpression>
#include <QFile>
#include <QTextCodec>
#include <QDebug>

ParsePython::ParsePython()
{

}

QList<LineContextItem> ParsePython::parse(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "cannot open" << path;
        return {};
    }

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF-8"));

    QRegularExpression class_("(\\s*)class\\s+([a-zA-Z_][a-zA-Z0-9_]*)", QRegularExpression::MultilineOption);
    QRegularExpression method("(\\s*)def\\s+([a-zA-Z_][a-zA-Z0-9_]*)\\s*[(](.*)[)]",
                              QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpression indent("(\\s*)[^\\s]", QRegularExpression::MultilineOption);

    // TODO escaping

    PythonHelper helper;

    PythonReader reader(stream);

    int lineNumber;

    while (!stream.atEnd()) {

        QPair<int, QString> line_ = reader.read();

        lineNumber = line_.first;
        QString line = line_.second;

        //qDebug() << lineNumber << line;

        QRegularExpressionMatch match_class = class_.match(line);
        QRegularExpressionMatch match_method = method.match(line);
        QRegularExpressionMatch match_indent = indent.match(line);

        if (match_class.hasMatch()) {

            QRegularExpressionMatch match = match_class;
            int indent = match.captured(1).size();
            QString name = match.captured(2);
            helper.startClass(lineNumber, indent, name);

            //qDebug() << "CLASS" << name;

        } else if (match_method.hasMatch()) {

            QRegularExpressionMatch match = match_method;
            int indent = match.captured(1).size();
            QString name = match.captured(2);

            QString signature = match.captured(3);
            signature.replace(QRegularExpression("\\s+"), " ");
            signature = "(" + signature.trimmed() + ")";

            helper.startMethod(lineNumber, indent, name + signature, name);

            //qDebug() << "METHOD" << name;

        } else if (match_indent.hasMatch()) {

            QRegularExpressionMatch match = match_indent;
            int indent = match.captured(1).size();
            helper.flush(indent, lineNumber);

        }

    }

    helper.flush(0, lineNumber);

    return helper.context();
}

PythonReader::PythonReader(QTextStream &stream) : stream(stream), lineNumber(0) {

}

QPair<int, QString> PythonReader::read() {

    QString line;
    int lineRead = 0;

    while (true) {

        line += stream.readLine() + "\n";
        lineRead++;

        int op_par = 0;
        int cl_par = 0;
        int op_br = 0;
        int cl_br = 0;

        State state = State::Code;

        for(int i=0;i<line.size();i++) {

            QChar c = line[i];

            if (state == State::Code) {

                if (c == '(') {
                    op_par++;
                } else if (c == ')') {
                    cl_par++;
                } else if (c == '[') {
                    op_br++;
                } else if (c == ']') {
                    cl_br++;
                } else if (c == '\'') {
                    state = State::SignleQuoteStr;
                } else if (c == '"') {
                    state = State::DoubleQuoteStr;
                } else if (c == '#') {
                    state = State::Comment;
                }

            } else if (state == State::SignleQuoteStr) {

                if (c == '\'') {
                    state = State::Code;
                }

            } else if (state == State::DoubleQuoteStr) {

                if (c == '"') {
                    state = State::Code;
                }

            } else if (state == State::HeredocStr) {

                if (c == '"') {
                    if (i-2 >= 0 && line[i-1] == '"' && line[i-2] == '"') {
                        state = State::Code;
                    }
                }

            } else if (state == State::Comment) {

                if (c == '\n') {
                    state = State::Code;
                }

            }
        }

        if ((state == State::Code) && op_par == cl_par && op_br == cl_br) {
            break;
        }

        if (stream.atEnd()) {
            break;
        }

    }


    QPair<int,QString> res(lineNumber, line);

    lineNumber += lineRead;

    return res;
}

PythonSymbol::PythonSymbol(PythonSymbol::Type type, const QString &name, const QString &shortName, int begin, int end, int indent)
    : type(type), name(name), shortName(shortName), begin(begin), end(end), indent(indent) {

}

bool PythonSymbol::contains(int line) const {
    return begin < line && line < end;
}

void PythonHelper::startClass(int lineNumber, int indent, const QString &name) {
    flush(indent, lineNumber - 1);
    incomplete[indent] = PythonSymbol(PythonSymbol::Class, name, name, lineNumber, -1, indent);
}

void PythonHelper::startMethod(int lineNumber, int indent, const QString &name, const QString &shortName) {
    flush(indent, lineNumber - 1);
    incomplete[indent] = PythonSymbol(PythonSymbol::Method, name, shortName, lineNumber, -1, indent);
}

void PythonHelper::flush(int indent, int lineNumber) {
    QList<int> keys = incomplete.keys();
    for(int key: keys) {
        if (key >= indent) {
            PythonSymbol symbol = incomplete[key];
            symbol.end = lineNumber;
            complete.append(symbol);
            incomplete.remove(key);
        }
    }
}

QList<LineContextItem> PythonHelper::context() const{


    //qDebug() << "complete.size()" << complete.size();

    QMap<int, QList<LineContextItem> > result;


    for(int i=0;i<complete.size();i++) {

        QList<PythonSymbol> stack;
        PythonSymbol symbol = complete[i];

        int lineNumber = symbol.begin + 1;

        for(int j=0;j<complete.size();j++) {
            if (complete[j].contains(lineNumber) || i == j) {
                stack.append(complete[j]);
            }
        }

        //qDebug() << i << stack.size();

        std::sort(stack.begin(), stack.end(), [](const PythonSymbol& s1, const PythonSymbol& s2){
            return s1.indent < s2.indent;
        });

        QStringList name;
        QStringList shortName;

        for(int k=0;k<stack.size();k++) {
            PythonSymbol s = stack[k];
            if (s.type == PythonSymbol::Class) {
                name.append(QString("class %1").arg(s.name));
                shortName.append(QString("class %1").arg(s.name));
            } else {
                QString name_ = QString("def %1").arg(s.name);
                QString shortName_ = QString("def %1").arg(s.shortName);
                if (k+1 < stack.size()) {
                    name.append(shortName_);
                    shortName.append(shortName_);
                } else {
                    // last item
                    name.append(name_);
                    shortName.append(shortName_);
                }
            }
        }

        if (!result.contains(stack.size())) {
            result[stack.size()] = QList<LineContextItem>();
        }

        QString name_ = name.join(" ");
        QString shortName_ = shortName.join(" ");
        int begin = stack.last().begin;
        int end = stack.last().end;

        result[stack.size()].append(LineContextItem(name_, shortName_, begin, end));

    }

    QList<int> keys = result.keys();
    std::sort(keys.begin(), keys.end(),[](int a, int b){return a >= b;});

    QList<LineContextItem> result_;
    for(int key: keys) {
        result_.append(result[key]);
    }
    return result_;
}
