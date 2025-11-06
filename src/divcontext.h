#ifndef DIVCONTEXT_H
#define DIVCONTEXT_H

#include <QString>
class Colors;

class DivContext {
public:

    DivContext() {

    }

    DivContext(const QString absolutePath, const  QString relativePath, int lineNumber, const QString& context);

    bool isNull() const {
        return absolutePath.isEmpty();
    }

    QString absolutePath;
    QString relativePath;
    int lineNumber;
    QString backgroundColor;
    QString color;
    QString context;

    QString render(const Colors&, bool showFileName, bool showLineNumber) const;
};


#endif // DIVCONTEXT_H
