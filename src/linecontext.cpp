#include "linecontext.h"

#include <QFileInfo>
#include <QByteArray>
#include <QDebug>
#include <QElapsedTimer>

LineContext::LineContext() : mInit(false)
{

}

class MatchingMap {
public:

    MatchingMap(const QList<QPair<int,int>>& par_op,
                const QList<QPair<int,int>>& par_cl,
                const QList<QPair<int,int>>& br_op,
                const QList<QPair<int,int>>& br_cl) : par_op(par_op), par_cl(par_cl), br_op(br_op), br_cl(br_cl) {

    }

    int matching(const QList<QPair<int,int>>& items, const QPair<int,int>& item, bool forward) {

        int inc = forward ? 1 : -1;
        int start = forward ? 0 : items.size() - 1;
        int stop = forward ? items.size() : -1;

        for(int i=start;i!=stop;i+=inc) {
            if (forward) {
                if (items[i].first <= item.first) {
                    continue;
                }
            } else {
                if (items[i].first >= item.first) {
                    continue;
                }
            }
            if (items[i].second != item.second) {
                continue;
            }
            return items[i].first;
        }

    }

    int indexOf(const QList<QPair<int,int>>& items, int pos) {
        for(int i=0;i<items.size();i++) {
            if (items[i].first == pos) {
                return i;
            }
        }
        return -1;
    }

    int matching(int pos) {
        int index = indexOf(par_op, pos);
        if (index > -1) {
            return matching(par_cl, par_op[index], true);
        }
        index = indexOf(par_cl, pos);
        if (index > -1) {
            return matching(par_op, par_cl[index], false);
        }
        index = indexOf(br_op, pos);
        if (index > -1) {
            return matching(br_cl, br_op[index], true);
        }
        index = indexOf(br_cl, pos);
        if (index > -1) {
            return matching(br_op, br_cl[index], false);
        }
    }

    QList<QPair<int,int>> par_op;
    QList<QPair<int,int>> par_cl;
    QList<QPair<int,int>> br_op;
    QList<QPair<int,int>> br_cl;

};

int pos_next(const QList<QPair<int,int>>& items, int pos) {
    for(int i=0;i<items.size();i++) {
        if (items[i].first < pos) {
            continue;
        }
        return items[i].first;
    }
    return -1;
}

bool has_init_list(const QString& content, int pos1, int pos2) {
    for(int i=pos1;i<=pos2;i++) {
        if (content[i] == ':') {
            return true;
        }
        bool ok = content[i] == ' '
                || content[i] == '\t'
                || content[i] == '\r'
                || content[i] == '\n';
        if (!ok) {
            return false;
        }
    }
    return false;
}

bool only_ids_and_whitespace_and_initlist(const QString& content, int pos1, int pos2) {

    bool init_list = has_init_list(content, pos1, pos2);

    for(int i=pos1;i<=pos2;i++) {
        bool ok1 = content[i].isDigit()
                || content[i].isLetter()
                || content[i] == '_'
                || content[i] == ' '
                || content[i] == '\t'
                || content[i] == '\r'
                || content[i] == '\n';

        bool ok2 = init_list && (
                    content[i] == '('
                    || content[i] == ')'
                    || content[i] == ','
                    || content[i] == ':'

                    );

        if (!ok1 && !ok2) {
            return false;
        }
    }
    return true;
}


int range_index(const QList<QPair<int,int> >& preproc_ranges, int pos) {
    for(int i=0;i<preproc_ranges.size();i++) {
        const auto& item = preproc_ranges[i];
        if (item.first <= pos && pos <= item.second) {
            return i;
        }
    }
    return -1;
}

int non_whitespace(const QString& content, int pos) {
    for(int i=pos;i<content.size();i++) {
        bool ws = content[i] == ' '
                || content[i] == '\t'
                || content[i] == '\r'
                || content[i] == '\n';
        if (!ws) {
            return i;
        }
    }
    return -1;
}

int head_method_name(const QString& content, int pos,
                     const QList<QPair<int,int> >& preproc_ranges,
                     const QList<QPair<int,int> >& singleline_comment_ranges,
                     const QList<QPair<int,int> >& multiline_comment_ranges) {
    for(int i=pos;i>=0;i--) {
        bool ok = content[i].isDigit()
                || content[i].isLetter()
                || content[i] == '_'
                || content[i] == ':'
                || content[i] == '<'
                || content[i] == '>'
                || content[i] == '*'
                || content[i] == '&'
                || content[i] == '~'
                || content[i] == ' '
                || content[i] == '\t'
                || content[i] == '\r'
                || content[i] == '\n'
                ;
        if (!ok) {
            int index1 = range_index(preproc_ranges, i);
            int index2 = range_index(singleline_comment_ranges, i);
            int index3 = range_index(multiline_comment_ranges, i);
            if (index1 > -1) {
                i = preproc_ranges[index1].second + 1;
            } else if (index2 > -1) {
                i = singleline_comment_ranges[index2].second + 1;
            } else if (index3 > -1) {
                i = multiline_comment_ranges[index3].second + 1;
            } else {
                i = i + 1;
            }
            return non_whitespace(content, i);
        }
    }
    return 0;
}

int line_number(const QList<int>& linebreaks, int pos) {
    for(int i=0;i<linebreaks.size();i++) {
        if (linebreaks[i] > pos) {
            return i;
        }
    }
    return linebreaks.size();
}

bool is_constructor_or_destructor_name(const QString& name) {

    QStringList cols = name.split("::");
    if (cols.size() != 2) {
        return false;
    }
    if (cols[0] == cols[1]) {
        return true;
    }
    QString col1 = "~" + cols[0];
    if (col1 == cols[1]) {
        return true;
    }
    return false;
}


void debug_print(const QString& content, int pos1, int pos2) {
    qDebug() << content.mid(pos1, pos2 - pos1 + 1);
}

#include <QRegularExpression>

void LineContext::parseCpp(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "cannot open" << path;
        return;
    }

    QString content = QString::fromUtf8(file.readAll());

    int par_level = 0;
    int br_level = 0;

    QList<QPair<int,int>> par_op;
    QList<QPair<int,int>> par_cl;
    QList<QPair<int,int>> br_op;
    QList<QPair<int,int>> br_cl;
    QList<int> linebreaks;

    int context_code = 0;
    int context_preproc = 1;
    int context_single_line_comment = 2;
    int context_multiline_comment = 3;

    int context = context_code;

    QList<QPair<int,int> > preproc_ranges;
    int preproc_start = -1;

    QList<QPair<int,int> > singleline_comment_ranges;
    QList<QPair<int,int> > multiline_comment_ranges;

    int singleline_comment_start = -1;
    int multiline_comment_start = -1;

    for(int i=0;i<content.size();i++) {

        if (context == context_code) {
            if (content[i] == '(') {
                par_op.append({i, par_level});
                par_level++;
            } else if (content[i] == ')') {
                par_level--;
                par_cl.append({i, par_level});
            } else if (content[i] == '{') {
                br_op.append({i, br_level});
                br_level++;
            } else if (content[i] == '}') {
                br_level--;
                br_cl.append({i, br_level});
            }

            if (content[i] == '/' && i + 1 < content.size() && content[i+1] == '/') {
                context = context_single_line_comment;
                singleline_comment_start = i;
            }

            if (content[i] == '/' && i + 1 < content.size() && content[i+1] == '*') {
                context = context_multiline_comment;
                multiline_comment_start = i;
            }
        }

        if (context == context_multiline_comment) {
            if (content[i] == '/' && i > 0 && content[i-1] == '*') {
                context = context_code;
                multiline_comment_ranges.append({multiline_comment_start, i});
            }
        }

        if (content[i] == '\n') {
            // TODO multiline preproc
            if (context == context_preproc) {
                preproc_ranges.append({preproc_start, i});
            }
            linebreaks.append(i);
            if (context != context_multiline_comment) {
                context = context_code;
            }
            if (singleline_comment_start > -1) {
                singleline_comment_ranges.append({singleline_comment_start, i});
                singleline_comment_start = -1;
            }

        } else if (content[i] == '#') {
            context = context_preproc;
            preproc_start = i;
        }
    }

    MatchingMap mm(par_op, par_cl, br_op, br_cl);

    QStringList skip = {"if", "for", "while", "else if"};

    //qDebug() << "names\n";

    for(int i=0;i<par_op.size();i++) {

        int pos_par_op = par_op[i].first;
        int pos_par_cl = mm.matching(pos_par_op);
        int pos_br_op = pos_next(br_op, pos_par_cl);

        if (pos_br_op < 0) {
            continue;
        }

        int pos_br_cl = mm.matching(pos_br_op);
        if (pos_br_cl < 0) {
            continue;
        }

        bool ok = only_ids_and_whitespace_and_initlist(content, pos_par_cl + 1, pos_br_op - 1);

        if (!ok) {
            continue;
        }

        int pos_begin = head_method_name(content, pos_par_op - 1, preproc_ranges, singleline_comment_ranges, multiline_comment_ranges);

        QString short_name = content.mid(pos_begin, pos_par_op - pos_begin);

        QString full_name = content.mid(pos_begin, pos_par_cl - pos_begin + 1);

        short_name = short_name.trimmed();

        short_name.replace(QRegularExpression("\\s+"), " ");

        full_name = full_name.trimmed();

        full_name.replace(QRegularExpression("\\s+"), " ");

        if (skip.indexOf(short_name) > -1) {
            continue;
        }

        if (short_name.startsWith(":")) {
            // skip initlist with one item
            continue;
        }

        if (short_name.contains(" ") || is_constructor_or_destructor_name(short_name)) {
            int begin = line_number(linebreaks, pos_begin);
            int end = line_number(linebreaks, pos_br_cl);
            mContext.append(LineContextItem(full_name, short_name, begin, end));
        }

    }

}

void LineContext::parsePy(const QString &path) {

}

void LineContext::init(const QString &path)
{
    if (mInit) {
        return;
    }
    mInit = true;

    QFileInfo info(path);

    QStringList cpp = {"cpp", "cxx", "h", "hpp"};
    QStringList py = {"py"};

    QString ext = info.suffix();

    if (cpp.contains(ext)) {
        parseCpp(path);
    }
    if (py.contains(ext)) {
        parsePy(path);
    }


}

QString LineContext::context(int line, bool signature) const
{
     for(int i=0;i<mContext.size();i++) {
         if (mContext[i].contains(line)) {
             return signature ? mContext[i].name : mContext[i].shortName;
         }
     }
     return QString();
}

void LineContext::dump()
{
    for(int i=0;i<mContext.size();i++) {
        const LineContextItem& item = mContext[i];
        qDebug() << "ITEM" << item.name << (item.begin + 1) << (item.end + 1);
    }
}

void LineContext::testParseCpp()
{
    auto PARSE_CPP_FILE = qgetenv("PARSE_CPP_FILE");
    if (PARSE_CPP_FILE.isNull()) {
        return;
    }
    LineContext context;
    QElapsedTimer timer;
    timer.start();
    context.init(QString::fromUtf8(PARSE_CPP_FILE));
    qDebug() << "context.init cpp" << timer.elapsed() << "ms";
    context.dump();
}
