// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Currency.hpp"

namespace ub {
namespace ledger {

/**
 * Return a map of supported currencies, where the index is the ISO 4217 code and the value is the
 * currency symbol.
 *
 * @return Map of supported currency symbols, indexed by ISO 4217 code
 */
const QMap<QString,QString>& symbols() {
    static QMap<QString,QString> codesToSymbols;

    // Only populate once
    if (codesToSymbols.size() == 0) {
        codesToSymbols.insert("ALL", QString("%1%2%3").arg(QChar(76)).arg(QChar(101)).arg(QChar(107))); // Albania Lek
        codesToSymbols.insert("AFN", QChar(1547)); // Afghanistan Afghani
        codesToSymbols.insert("ARS", QChar(36)); // Argentina Peso
        codesToSymbols.insert("AWG", QChar(402)); // Aruba Guilder
        codesToSymbols.insert("AUD", QChar(36)); // Australia Dollar
        codesToSymbols.insert("AZN", QString("%1%2%3").arg(QChar(1084)).arg(QChar(1072)).arg(QChar(1085))); // Azerbaijan New Manat

        codesToSymbols.insert("BSD", QChar(36)); // Bahamas Dollar
        codesToSymbols.insert("BBD", QChar(36)); // Barbados Dollar
        codesToSymbols.insert("BYR", QString("%1%2").arg(QChar(112)).arg(QChar(46))); // Belarus Ruble
        codesToSymbols.insert("BZD", QString("%1%2%3").arg(QChar(66)).arg(QChar(90)).arg(QChar(36))); // Belize Dollar
        codesToSymbols.insert("BMD", QChar(36)); // Bermuda Dollar
        codesToSymbols.insert("BOB", QString("%1%2").arg(QChar(36)).arg(QChar(98))); // Bolivia Boliviano
        codesToSymbols.insert("BAM", QString("%1%2").arg(QChar(75)).arg(QChar(77))); // Bosnia & Herzegovina Convertible Marka
        codesToSymbols.insert("BWP", QChar(80)); // Botswana Pula
        codesToSymbols.insert("BGN", QString("%1%2").arg(QChar(1083)).arg(QChar(1074))); // Bulgaria Lev
        codesToSymbols.insert("BRL", QString("%1%2").arg(QChar(82)).arg(QChar(36))); // Brazil Real
        codesToSymbols.insert("BND", QChar(36)); // Brunei Darussalam Dollar

        codesToSymbols.insert("KHR", QChar(6107)); // Cambodia Riel
        codesToSymbols.insert("CAD", QChar(36)); // Canada Dollar
        codesToSymbols.insert("KYD", QChar(36)); // Cayman Islands Dollar
        codesToSymbols.insert("CLP", QChar(36)); // Chile Peso
        codesToSymbols.insert("CNY", QChar(165)); // China Yuan Renminbi
        codesToSymbols.insert("COP", QChar(36)); // Colombia Peso
        codesToSymbols.insert("CRC", QChar(8353)); // Costa Rica Colon
        codesToSymbols.insert("HRK", QString("%1%2").arg(QChar(107)).arg(QChar(110))); // Croatia Kuna
        codesToSymbols.insert("CUP", QChar(8369)); // Cuba Peso
        codesToSymbols.insert("CZK", QString("%1%2").arg(QChar(75)).arg(QChar(269))); // Czech Republic Koruna

        codesToSymbols.insert("DKK", QString("%1%2").arg(QChar(107)).arg(QChar(114))); // Denmark Krone
        codesToSymbols.insert("DOP", QString("%1%2%3").arg(QChar(82)).arg(QChar(68)).arg(QChar(36))); // Dominical Republic Peso

        codesToSymbols.insert("XCD", QChar(36)); // East Caribbean Dollar
        codesToSymbols.insert("EGP", QChar(163)); // Egypt Pound
        codesToSymbols.insert("SVC", QChar(36)); // El Salvador Colon
        codesToSymbols.insert("EEK", QString("%1%2").arg(QChar(107)).arg(QChar(114))); // Estonia Kroon
        codesToSymbols.insert("EUR", QChar(8364)); // Euro Member Countries

        codesToSymbols.insert("FKP", QChar(163)); // Falkland Islands (Malvinas) Pound
        codesToSymbols.insert("FJD", QChar(36)); // Fiji Dollar

        codesToSymbols.insert("GHC", QChar(162)); // Ghana Cedis
        codesToSymbols.insert("GIP", QChar(163)); // Gibraltar Pound
        codesToSymbols.insert("GTQ", QChar(81)); // Guatemala Quetzal
        codesToSymbols.insert("GGP", QChar(163)); // Guernsey Pound
        codesToSymbols.insert("GYD", QChar(36)); // Guyana Dollar

        codesToSymbols.insert("HNL", QChar(76)); // Honduras Lempira
        codesToSymbols.insert("HKD", QChar(36)); // Hong Kong Dollar
        codesToSymbols.insert("HUF", QString("%1%2").arg(QChar(70)).arg(QChar(116))); // Hungary Forint

        codesToSymbols.insert("ISK", QString("%1%2").arg(QChar(107)).arg(QChar(114))); // Iceland Krona
        codesToSymbols.insert("INR", QChar(8425)); // India Rupee
        codesToSymbols.insert("IDR", QString("%1%2").arg(QChar(82)).arg(QChar(112))); // Indonesia Rupiah
        codesToSymbols.insert("IRR", QChar(65020)); // Iran Rial
        codesToSymbols.insert("IMP", QChar(163)); // Isle of Man Pound
        codesToSymbols.insert("ILS", QChar(8362)); // Israel Shekel

        codesToSymbols.insert("JMD", QString("%1%2").arg(QChar(74)).arg(QChar(36))); // Jamaican Dollar
        codesToSymbols.insert("JPY", QChar(165)); // Japan Yen
        codesToSymbols.insert("JEP", QChar(163)); // Jersey Pound

        codesToSymbols.insert("KZT", QString("%1%2").arg(QChar(1083)).arg(QChar(1074))); // Kazakhstan Tenge
        codesToSymbols.insert("KPW", QChar(8361)); // Korea (North) Won
        codesToSymbols.insert("KRW", QChar(8361)); // Korea (South) Won
        codesToSymbols.insert("KGS", QString("%1%2").arg(QChar(1083)).arg(QChar(1074))); // Kyrgyzstan Som

        codesToSymbols.insert("LAK", QChar(8365)); // Laos Kip
        codesToSymbols.insert("LVL", QString("%1%2").arg(QChar(76)).arg(QChar(115))); // Latvia Lat
        codesToSymbols.insert("LBP", QChar(163)); // Lebanon Pound
        codesToSymbols.insert("LRD", QChar(36)); // Liberia Dollar
        codesToSymbols.insert("LTL", QString("%1%2").arg(QChar(76)).arg(QChar(116))); // Lithuania Litas

        codesToSymbols.insert("MKD", QString("%1%2%3").arg(QChar(1076)).arg(QChar(1077)).arg(QChar(1085))); // Macedonia Denar
        codesToSymbols.insert("MYR", QString("%1%2").arg(QChar(82)).arg(QChar(77))); // Malaysia Ringgit
        codesToSymbols.insert("MUR", QChar(8360)); // Mauritius Rupee
        codesToSymbols.insert("MXN", QChar(36)); // Mexico Peso
        codesToSymbols.insert("MNT", QChar(8366)); // Mongolia Tughrik
        codesToSymbols.insert("MZN", QString("%1%2").arg(QChar(77)).arg(QChar(84))); // Mozambique Metical

        codesToSymbols.insert("NAD", QChar(36)); // Namibia Dollar
        codesToSymbols.insert("NPR", QChar(8360)); // Nepal Rupee
        codesToSymbols.insert("ANG", QChar(402)); // Netherlands Antilles Guilder
        codesToSymbols.insert("NZD", QChar(36)); // New Zealand Dollar
        codesToSymbols.insert("NIO", QString("%1%2").arg(QChar(67)).arg(QChar(36))); // Nicaragua Cordoba
        codesToSymbols.insert("NGN", QChar(8358)); // Nigeria Naira
        codesToSymbols.insert("NOK", QString("%1%2").arg(QChar(107)).arg(QChar(114))); // Norway Krone

        codesToSymbols.insert("OMR", QChar(65020)); // Oman Rial

        codesToSymbols.insert("PKR", QChar(8360)); // Pakistan Rupee
        codesToSymbols.insert("PAB", QString("%1%2%3").arg(QChar(66)).arg(QChar(47)).arg(QChar(46))); // Panama Balboa
        codesToSymbols.insert("PYG", QString("%1%2").arg(QChar(71)).arg(QChar(115))); // Paraguay Guarani
        codesToSymbols.insert("PEN", QString("%1%2%3").arg(QChar(83)).arg(QChar(47)).arg(QChar(46))); // Peru Nuevo Sol
        codesToSymbols.insert("PHP", QChar(8369)); // Philippines Peso
        codesToSymbols.insert("PLN", QString("%1%2").arg(QChar(122)).arg(QChar(322))); // Poland Zloty

        codesToSymbols.insert("QAR", QChar(65020)); // Qatar Riyal

        codesToSymbols.insert("RON", QString("%1%2%3").arg(QChar(108)).arg(QChar(101)).arg(QChar(105))); // Romania New Leu
        codesToSymbols.insert("RUB", QString("%1%2%3").arg(QChar(1088)).arg(QChar(1091)).arg(QChar(1073))); // Russia Ruble

        codesToSymbols.insert("SHP", QChar(163)); // Saint Helena Pound
        codesToSymbols.insert("SAR", QChar(65020)); // Saudi Arabia Riyal
        codesToSymbols.insert("RSD", QString("%1%2%3%4").arg(QChar(1044)).arg(QChar(1080)).arg(QChar(1085)).arg(QChar(46))); // Serbia Dinar
        codesToSymbols.insert("SCR", QChar(8360)); // Seychelles Rupee
        codesToSymbols.insert("SGD", QChar(36)); // Singapore Dollar
        codesToSymbols.insert("SBD", QChar(36)); // Solomon Islands Dollar
        codesToSymbols.insert("SOS", QChar(83)); // Somalia Shilling
        codesToSymbols.insert("ZAR", QChar(82)); // South Africa Rand
        codesToSymbols.insert("LKR", QChar(8360)); // Sri Lanka Rupee
        codesToSymbols.insert("SEK", QString("%1%2").arg(QChar(107)).arg(QChar(114))); // Sweden Krona
        codesToSymbols.insert("CHF", QString("%1%2%3").arg(QChar(67)).arg(QChar(72)).arg(QChar(70))); // Switzerland Franc
        codesToSymbols.insert("SRD", QChar(36)); // Suriname Dollar
        codesToSymbols.insert("SYP", QChar(163)); // Syria Pound

        codesToSymbols.insert("TWD", QString("%1%2%3").arg(QChar(78)).arg(QChar(84)).arg(QChar(36))); // Taiwan New Dollar
        codesToSymbols.insert("THB", QChar(3647)); // Thailand Baht
        codesToSymbols.insert("TTD", QString("%1%2%3").arg(QChar(84)).arg(QChar(84)).arg(QChar(36))); // Trinidad and Tobago Dollar
        codesToSymbols.insert("TRY", QChar(8378)); // Turkey Lira
        codesToSymbols.insert("TRL", QChar(8356)); // Turkey Lira
        codesToSymbols.insert("TVD", QChar(36)); // Tuvalu Dollar

        codesToSymbols.insert("UAH", QChar(8372)); // Ukraine Hryvna
        codesToSymbols.insert("GBP", QChar(163)); // United Kingdom Pound
        codesToSymbols.insert("USD", QChar(36)); // United States Dollar
        codesToSymbols.insert("UYU", QString("%1%2").arg(QChar(36)).arg(QChar(85))); // Uruguay Peso
        codesToSymbols.insert("UZS", QString("%1%2").arg(QChar(1083)).arg(QChar(1074))); // Uzbekistan Som

        codesToSymbols.insert("VEF", QString("%1%2").arg(QChar(66)).arg(QChar(115))); // Venezuela Bolivar
        codesToSymbols.insert("VND", QChar(8363)); // Viet Nam Dong

        codesToSymbols.insert("YER", QChar(65020)); // Yemen Rial

        codesToSymbols.insert("ZWD", QString("%1%2").arg(QChar(90)).arg(QChar(36))); // Zimbabwe Dollar
    }

    return codesToSymbols;
}

//--------------------------------------------------------------------------------------------------
Currency::Currency() : id_(1), iso4217_(QLocale().currencySymbol(QLocale::CurrencyIsoCode)) {}

//--------------------------------------------------------------------------------------------------
Currency::Currency(int id, const QString & code) : id_(id), iso4217_(code) {}

//--------------------------------------------------------------------------------------------------
bool Currency::operator==(const Currency & that) const {
    return (this->iso4217_ == that.iso4217_);
}

//--------------------------------------------------------------------------------------------------
bool Currency::operator!=(const Currency & that) const {
    return (this->iso4217_ != that.iso4217_);
}

//--------------------------------------------------------------------------------------------------
QString Currency::code() const {
    return iso4217_;
}

//--------------------------------------------------------------------------------------------------
QString Currency::format(double value) const {
    return QLocale().toCurrencyString(value, symbol());
}

//--------------------------------------------------------------------------------------------------
int Currency::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
QString Currency::symbol() const {
    return symbols().value(iso4217_, QString('?'));
}

} // ledger namespace
} // ub namespace
