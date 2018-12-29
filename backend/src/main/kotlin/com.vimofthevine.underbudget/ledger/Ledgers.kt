package com.vimofthevine.underbudget.ledger

import java.util.UUID

import org.jetbrains.exposed.dao.EntityID
import org.jetbrains.exposed.dao.UUIDEntity
import org.jetbrains.exposed.dao.UUIDEntityClass
import org.jetbrains.exposed.dao.UUIDTable
import org.jetbrains.exposed.sql.SchemaUtils

object Ledgers : UUIDTable("ledger") {
    val name = varchar("name", 128)
    val defaultCurrency = integer("default_currency")
    val time = datetime("time")
}

class LedgerDao(id: EntityID<UUID>) : UUIDEntity(id) {
    companion object : UUIDEntityClass<LedgerDao>(Ledgers)
    
    var name by Ledgers.name
    private var defaultCurrencyCode by Ledgers.defaultCurrency
    var defaultCurrency
    	get() = Currencies.get(defaultCurrencyCode)
    	set(value) { defaultCurrencyCode = value.getNumericCode() }
    var time by Ledgers.time
}

fun setupLedgerTables() : Unit {
    SchemaUtils.create(Accounts, Envelopes, Ledgers)
}