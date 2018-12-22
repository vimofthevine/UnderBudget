package com.vimofthevine.underbudget

import java.util.Locale

import javax.money.Monetary
import javax.money.format.AmountFormatQueryBuilder
import javax.money.format.MonetaryFormats

import org.javamoney.moneta.Money
import org.javamoney.moneta.format.CurrencyStyle

fun main(args: Array<String>) {
    println("UnderBudget v0.0.1")
    
    val usd = Monetary.getCurrency("USD")
    val amount = Money.of(1200.75, usd)
    println("Currency is $usd")
    println("Amount is $amount")
    
    val diff = amount.subtract(Money.of(744.30, usd))
    println("Diff is $diff")
    
    val formatter = MonetaryFormats.getAmountFormat(
        AmountFormatQueryBuilder.of(Locale.US)
            .set(CurrencyStyle.SYMBOL)
            .build())
    println("Formatted is ${formatter.format(diff)}")
}