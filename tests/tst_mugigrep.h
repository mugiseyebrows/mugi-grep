#ifndef TST_MUGIGREP_H
#define TST_MUGIGREP_H
#include <QObject>
class tst_MugiGrep : public QObject
{
    Q_OBJECT
public:
    explicit tst_MugiGrep(QObject *parent = 0);
private slots:
    void initTestCase();
    void cleanupTestCase();
    void searchSingleline();
    void searchMultiline();
};
#endif // TST_MUGIGREP

