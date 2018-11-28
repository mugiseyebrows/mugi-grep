#ifndef SL_H
#define SL_H

#include <QStringList>

/*

[
"QStringList sl(",
["const QString& s%i"], ", ",
")\n{\nQStringList res;\n",
["res << s%i;\n"],"",
"return res;\n}\n"
]

[
"QStringList sl(",
["const QString& s%i"], ", ",
");\n"
]

*/

QStringList sl(const QString& s0);
QStringList sl(const QString& s0, const QString& s1);
QStringList sl(const QString& s0, const QString& s1, const QString& s2);
QStringList sl(const QString& s0, const QString& s1, const QString& s2, const QString& s3);
QStringList sl(const QString& s0, const QString& s1, const QString& s2, const QString& s3, const QString& s4);
QStringList sl(const QString& s0, const QString& s1, const QString& s2, const QString& s3, const QString& s4, const QString& s5);
QStringList sl(const QString& s0, const QString& s1, const QString& s2, const QString& s3, const QString& s4, const QString& s5, const QString& s6);
QStringList sl(const QString& s0, const QString& s1, const QString& s2, const QString& s3, const QString& s4, const QString& s5, const QString& s6, const QString& s7);
QStringList sl(const QString& s0, const QString& s1, const QString& s2, const QString& s3, const QString& s4, const QString& s5, const QString& s6, const QString& s7, const QString& s8);
QStringList sl(const QString& s0, const QString& s1, const QString& s2, const QString& s3, const QString& s4, const QString& s5, const QString& s6, const QString& s7, const QString& s8, const QString& s9);
QStringList sl(const QString& s0, const QString& s1, const QString& s2, const QString& s3, const QString& s4, const QString& s5, const QString& s6, const QString& s7, const QString& s8, const QString& s9, const QString& s10);
QStringList sl(const QString& s0, const QString& s1, const QString& s2, const QString& s3, const QString& s4, const QString& s5, const QString& s6, const QString& s7, const QString& s8, const QString& s9, const QString& s10, const QString& s11);


#endif // SL_H
