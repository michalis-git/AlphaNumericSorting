#include <QCoreApplication>
#include <QDebug>
#include <algorithm>
#include <vector>
#include <string>


// has digits and then letters
bool hasDigitsFirst(const QString &s, int &digits) {
  bool isMixed = false;
  QString num;
  for(QChar c : s) {
    if (c.isDigit() || c == '.' || c == '-') {
      num.append(c);
    } else {
      if (num.length()) {
        isMixed = true;
      }
      break;
    }
  }
  digits = num.toInt();
  return isMixed;
}

// has letters and then digits
bool hasLettersFirst(const QString &s, QString &letters) {
  bool isMixed = false;
  QString num;
  for(QChar c : s) {
    if (!c.isDigit()) {
      num.append(c);
    } else {
      if (num.length()) {
        isMixed = true;
        letters = num;
      }
      break;
    }
  }
  return isMixed;
}

int digitsAfterLettersToInt(const QString &lettersPart, const QString &string) {
  return string.midRef(lettersPart.length(), string.length()).toInt();
}

bool compare(const QString &l, const QString &r, const int &lfPart, const int &rfPart) {
  // if numeric parts is equal then put the shorter string first
  if (lfPart == rfPart)
    return l < r;
  // else put the smaller number first
  else
    return lfPart < rfPart;
}

bool compare(const QString &l, const QString &r,
             const int &lDig, const int &rDig,
             const QString &lLet, const QString &rLet) {
  if (lLet == rLet) {
//    qDebug() << digitsAfterLettersToInt(lLet, l) << digitsAfterLettersToInt(rLet, r);
    return digitsAfterLettersToInt(lLet, l) < digitsAfterLettersToInt(rLet, r);
  }
  // else put the smaller number first
  else
    return lDig < rDig;
}

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

     //  QStringList v { "2", "b", "1", "a", "10" , "2a", "1a"};
     //  QStringList v { "1028", "1029", "103", "1030", "1031" , "2a", "1a"};
  QStringList v { "1", "1028", "1029", "103", "1030", "10a", "1031" , "2a", "1a", "a1", "a10", "a5"};
  // has to become: 1, 1a, 2, 2a, 10, a, b

  auto numericCompare = [](const QString &l, const QString &r) {

    bool lb, rb;
    int li = l.toInt(&lb);
    int ri = r.toInt(&rb);

       // if both are numbers
    if (lb && rb) {
      return li < ri;
    }

    int liDigits, riDigits = 0;
    QString lLetters, rLetters;
    bool lHasDigitsFirst = hasDigitsFirst(l, liDigits);
    bool rHasDigitsFirst = hasDigitsFirst(r, riDigits);
    bool lHasLettersFirst = hasLettersFirst(l, lLetters);
    bool rHasLettersFirst = hasLettersFirst(r, rLetters);

    qDebug() << l << "->" << li << " , " << r << "->" << ri
        //             << "lHasDigitsFirst: " << lHasDigitsFirst << ", "
        //             << "rHasDigitsFirst: " << rHasDigitsFirst << ", "
        //             << "lHasLettersFirst: " << lHasLettersFirst << ", "
        //             << "rHasLettersFirst: " << rHasLettersFirst << ", "
        ;

    //    if both are mixed-digits-first
    if (lHasDigitsFirst && rHasDigitsFirst) {
      //      qDebug() << l << r << "both have DIGITS first";
      return compare(l, r, liDigits, riDigits);
    }

    /***************** if both are mixed-letters-first *****************/
    else if (lHasLettersFirst && rHasLettersFirst) {
//      qDebug() << l << r << "both have LETTERS first" << lLetters << rLetters;
      return compare(l, r, liDigits, riDigits, lLetters, rLetters);
    }

    //    // if one is number and other mixed-letters-first
    //    else if ((lb && rHasLettersFirst) || (lHasLettersFirst && rb)) {
    //      return l < r;
    //    }

         // if one is number and other mixed-digits-first
    else if ((lb && rHasDigitsFirst) || (lHasDigitsFirst && rb)) {
      //      qDebug() << l << r << "one is DIGITS and other has LETTERS first" << li << ri;
      return compare(l, r, liDigits, riDigits);
    }

    return l < r;
  };

  std::sort(v.begin(), v.end(), numericCompare);
  qDebug() << v;

  return a.exec();
}

//    std::string lstr = l.toStdString();
//    std::string rstr = r.toStdString();

//    int li = std::stoi(l);
//    int ri = std::stoi(r);
//    if (li && ri)
//      return li < ri;
//    return l < r;

//    for (auto &i : v)
//    qDebug() << i.c_str();
