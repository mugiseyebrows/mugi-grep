#ifndef LIT_H
#define LIT_H
#include <QList>
#include <QString>
#include <QVariant>

namespace Lit {
    QList<int> il();
    QList<int> il(int v0);
    QList<int> il(int v0, int v1);
    QList<int> il(int v0, int v1, int v2);
    QList<int> il(int v0, int v1, int v2, int v3);
    QList<int> il(int v0, int v1, int v2, int v3, int v4);
    QList<int> il(int v0, int v1, int v2, int v3, int v4, int v5);
    QList<int> il(int v0, int v1, int v2, int v3, int v4, int v5, int v6);
    QList<int> il(int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7);
    QList<int> il(int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8);
    QList<int> il(int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9);
    QList<bool> bl();
    QList<bool> bl(bool v0);
    QList<bool> bl(bool v0, bool v1);
    QList<bool> bl(bool v0, bool v1, bool v2);
    QList<bool> bl(bool v0, bool v1, bool v2, bool v3);
    QList<bool> bl(bool v0, bool v1, bool v2, bool v3, bool v4);
    QList<bool> bl(bool v0, bool v1, bool v2, bool v3, bool v4, bool v5);
    QList<bool> bl(bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6);
    QList<bool> bl(bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7);
    QList<bool> bl(bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7, bool v8);
    QList<bool> bl(bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7, bool v8,
                   bool v9);
    QList<double> dl();
    QList<double> dl(double v0);
    QList<double> dl(double v0, double v1);
    QList<double> dl(double v0, double v1, double v2);
    QList<double> dl(double v0, double v1, double v2, double v3);
    QList<double> dl(double v0, double v1, double v2, double v3, double v4);
    QList<double> dl(double v0, double v1, double v2, double v3, double v4, double v5);
    QList<double> dl(double v0, double v1, double v2, double v3, double v4, double v5, double v6);
    QList<double> dl(double v0, double v1, double v2, double v3, double v4, double v5, double v6,
                     double v7);
    QList<double> dl(double v0, double v1, double v2, double v3, double v4, double v5, double v6,
                     double v7, double v8);
    QList<double> dl(double v0, double v1, double v2, double v3, double v4, double v5, double v6,
                     double v7, double v8, double v9);
    QList<QString> sl();
    QList<QString> sl(const QString& v0);
    QList<QString> sl(const QString& v0, const QString& v1);
    QList<QString> sl(const QString& v0, const QString& v1, const QString& v2);
    QList<QString> sl(const QString& v0, const QString& v1, const QString& v2, const QString& v3);
    QList<QString> sl(const QString& v0, const QString& v1, const QString& v2, const QString& v3,
                      const QString& v4);
    QList<QString> sl(const QString& v0, const QString& v1, const QString& v2, const QString& v3,
                      const QString& v4, const QString& v5);
    QList<QString> sl(const QString& v0, const QString& v1, const QString& v2, const QString& v3,
                      const QString& v4, const QString& v5, const QString& v6);
    QList<QString> sl(const QString& v0, const QString& v1, const QString& v2, const QString& v3,
                      const QString& v4, const QString& v5, const QString& v6, const QString& v7);
    QList<QString> sl(const QString& v0, const QString& v1, const QString& v2, const QString& v3,
                      const QString& v4, const QString& v5, const QString& v6, const QString& v7,
                      const QString& v8);
    QList<QString> sl(const QString& v0, const QString& v1, const QString& v2, const QString& v3,
                      const QString& v4, const QString& v5, const QString& v6, const QString& v7,
                      const QString& v8, const QString& v9);
    QList<QVariant> vl();
    QList<QVariant> vl(const QVariant& v0);
    QList<QVariant> vl(const QVariant& v0, const QVariant& v1);
    QList<QVariant> vl(const QVariant& v0, const QVariant& v1, const QVariant& v2);
    QList<QVariant> vl(const QVariant& v0, const QVariant& v1, const QVariant& v2,
                       const QVariant& v3);
    QList<QVariant> vl(const QVariant& v0, const QVariant& v1, const QVariant& v2,
                       const QVariant& v3, const QVariant& v4);
    QList<QVariant> vl(const QVariant& v0, const QVariant& v1, const QVariant& v2,
                       const QVariant& v3, const QVariant& v4, const QVariant& v5);
    QList<QVariant> vl(const QVariant& v0, const QVariant& v1, const QVariant& v2,
                       const QVariant& v3, const QVariant& v4, const QVariant& v5,
                       const QVariant& v6);
    QList<QVariant> vl(const QVariant& v0, const QVariant& v1, const QVariant& v2,
                       const QVariant& v3, const QVariant& v4, const QVariant& v5,
                       const QVariant& v6, const QVariant& v7);
    QList<QVariant> vl(const QVariant& v0, const QVariant& v1, const QVariant& v2,
                       const QVariant& v3, const QVariant& v4, const QVariant& v5,
                       const QVariant& v6, const QVariant& v7, const QVariant& v8);
    QList<QVariant> vl(const QVariant& v0, const QVariant& v1, const QVariant& v2,
                       const QVariant& v3, const QVariant& v4, const QVariant& v5,
                       const QVariant& v6, const QVariant& v7, const QVariant& v8,
                       const QVariant& v9);
} // namespace Lit
#endif // LIT_H
