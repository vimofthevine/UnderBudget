package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.DbService

import java.util.UUID

import org.jetbrains.exposed.sql.*
import org.joda.time.DateTime

data class Ledger(val name: String, val defaultCurrency: String, val id: UUID? = null)

fun DbService.create(ledger: Ledger): UUID =
    Ledgers.insertAndGetId {
        it[name] = ledger.name
        it[defaultCurrency] = Currencies.get(ledger.defaultCurrency).getNumericCode()
        it[time] = DateTime()
    }.value

fun DbService.getLedgers(): List<Ledger> = Ledgers.selectAll().mapNotNull { toLedger(it) }

fun DbService.getLedger(id: UUID): Ledger? =
	Ledgers.select { Ledgers.id eq id }.mapNotNull { toLedger(it) }.singleOrNull()

fun DbService.toLedger(row: ResultRow) = Ledger(
    name = row[Ledgers.name],
    defaultCurrency = Currencies.get(row[Ledgers.defaultCurrency]).getCurrencyCode(),
    id = row[Ledgers.id].value
)