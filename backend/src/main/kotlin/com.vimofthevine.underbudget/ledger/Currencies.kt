package com.vimofthevine.underbudget.ledger

import javax.money.CurrencyUnit
import javax.money.Monetary

object Currencies {
    private val usd = Monetary.getCurrency("USD")
    
    private val cache: Map<Int, CurrencyUnit> = Monetary.getCurrencies().filter {
        (it.getNumericCode() > 0) && (it.getCurrencyCode() !in arrayOf<String>("ROL", "YUM"))
    }.associateBy { it.getNumericCode() }
    
    fun get(num: Int): CurrencyUnit = cache.getOrDefault(num, usd)
    
    fun get(code: String): CurrencyUnit = Monetary.getCurrency(code)
}