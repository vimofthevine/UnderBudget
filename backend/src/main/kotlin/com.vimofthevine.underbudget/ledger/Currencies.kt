package com.vimofthevine.underbudget.ledger

import javax.money.CurrencyUnit
import javax.money.Monetary
import javax.money.UnknownCurrencyException

object Currencies {
    private val usd = Monetary.getCurrency("USD")
    
    private val cache: Map<Int, CurrencyUnit> = Monetary.getCurrencies().filter {
        (it.getNumericCode() > 0) && (it.getCurrencyCode() !in arrayOf<String>("ROL", "YUM"))
    }.associateBy { it.getNumericCode() }
    
    fun get(num: Int): CurrencyUnit = cache.getOrDefault(num, usd)
    
    fun get(code: String): CurrencyUnit =  try {
        Monetary.getCurrency(code)
    } catch (e: UnknownCurrencyException) {
        usd
    }

    fun isValid(code: String): Boolean = try {
        Monetary.getCurrency(code)
        true
    } catch (e: UnknownCurrencyException) {
        false
    }
}