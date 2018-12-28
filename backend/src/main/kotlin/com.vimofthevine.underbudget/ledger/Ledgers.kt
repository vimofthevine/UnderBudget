package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.auth.Household
import com.vimofthevine.underbudget.auth.Households

import java.util.UUID

import org.jetbrains.exposed.dao.EntityID
import org.jetbrains.exposed.dao.UUIDEntity
import org.jetbrains.exposed.dao.UUIDEntityClass
import org.jetbrains.exposed.dao.UUIDTable
import org.jetbrains.exposed.sql.SchemaUtils

object Ledgers : UUIDTable("ledger") {
    val householdId = reference("household_id", Households).uniqueIndex()
    val rootAccountId = reference("root_account_id", Accounts).uniqueIndex()
    val defaultCurrency = integer("default_currency")
}

class Ledger(id: EntityID<UUID>) : UUIDEntity(id) {
    companion object : UUIDEntityClass<Ledger>(Ledgers)
    
    var household by Household referencedOn Ledgers.householdId
    var rootAccount by Account referencedOn Ledgers.rootAccountId
    private var defaultCurrencyCode by Ledgers.defaultCurrency
    var defaultCurrency
    	get() = Currencies.get(defaultCurrencyCode)
    	set(value) { defaultCurrencyCode = value.getNumericCode() }
}

fun setupLedgerTables() : Unit {
    SchemaUtils.create(Accounts, Ledgers)
}