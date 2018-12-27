package com.vimofthevine.underbudget.ledger

import javax.money.Monetary

import org.jetbrains.exposed.dao.EntityID
import org.jetbrains.exposed.dao.IntEntity
import org.jetbrains.exposed.dao.IntEntityClass
import org.jetbrains.exposed.dao.IntIdTable
import org.jetbrains.exposed.sql.SchemaUtils
import org.jetbrains.exposed.sql.batchInsert
import org.jetbrains.exposed.sql.selectAll

object Currencies : IntIdTable("currency") {
    val code = varchar("code", 5)
}

class Currency(id: EntityID<Int>) : IntEntity(id) {
    companion object : IntEntityClass<Currency>(Currencies)
    
    val code by Currencies.code
    val unit by lazy {
        Monetary.getCurrency(code)
    }
}

fun setupCurrencyTable() : Unit {
    SchemaUtils.create(Currencies)
    if (Currencies.selectAll().count() == 0) {
        Currencies.batchInsert(Monetary.getCurrencies()) {
            this[Currencies.id] = EntityID<Int>(it.getNumericCode(), Currencies)
            this[Currencies.code] = it.getCurrencyCode()
        }
    }
}