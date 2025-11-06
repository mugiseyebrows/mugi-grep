#ifndef DIVS_H
#define DIVS_H

#include "divcontext.h"
#include "divhit.h"

class Divs {
public:
    void append(const DivContext& context) {
        divs.append(QPair<DivContext, DivHit>(context, DivHit()));
    }

    void append(const DivHit& hit) {
        divs.append(QPair<DivContext, DivHit>(DivContext(), hit));
    }

    QString render(const Colors& colors, bool showFileName, bool showLineNumber) {
        QStringList res;
        for(int i=0;i<divs.size();i++) {
            const auto& item = divs[i];
            const DivContext& context = item.first;
            const DivHit& hit = item.second;
            if (!context.isNull()) {
                res << context.render(colors, showFileName, showLineNumber);
            } else {
                res << hit.render(colors, showFileName, showLineNumber);
            }

        }
        return res.join("");
    }

    QList<QPair<DivContext, DivHit> > divs;
};

class Divs2 {
public:

    void append(const DivContext& context) {
        divs.append(QPair<DivContext, DivHit2>(context, DivHit2()));
    }

    void append(const DivHit2& hit) {
        divs.append(QPair<DivContext, DivHit2>(DivContext(), hit));
    }

    QString render(const Colors& colors, bool showFileName, bool showLineNumber) {
        QStringList res;
        for(int i=0;i<divs.size();i++) {
            const auto& item = divs[i];
            const DivContext& context = item.first;
            const DivHit2& hit = item.second;
            if (!context.isNull()) {
                res << context.render(colors, showFileName, showLineNumber);
            } else {
                res << hit.render(colors, showFileName, showLineNumber);
            }

        }
        return res.join("");
    }


protected:

    QList<QPair<DivContext, DivHit2> > divs;
};

#endif // DIVS_H
