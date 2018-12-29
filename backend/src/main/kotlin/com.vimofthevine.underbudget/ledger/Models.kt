package com.vimofthevine.underbudget.ledger

import java.util.UUID

import org.jetbrains.exposed.sql.*
import org.joda.time.DateTime

data class Ledger(val name: String, val defaultCurrency: String, val id: UUID? = null)

class LedgerService(val db: Database) {
    
    fun create(ledger: Ledger): UUID =
        Ledgers.insertAndGetId {
            it[name] = ledger.name
            it[defaultCurrency] = Currencies.get(ledger.defaultCurrency).getNumericCode()
            it[time] = DateTime()
        }.value
    
    fun getLedgers(): List<Ledger> = Ledgers.selectAll().mapNotNull { toLedger(it) }
    
    private fun toLedger(row: ResultRow) = Ledger(
        name = row[Ledgers.name],
        defaultCurrency = Currencies.get(row[Ledgers.defaultCurrency]).getCurrencyCode(),
        id = row[Ledgers.id].value
    )
}