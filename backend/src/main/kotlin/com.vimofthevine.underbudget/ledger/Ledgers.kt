package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.auth.Household
import com.vimofthevine.underbudget.auth.Households

import java.util.UUID

import org.jetbrains.exposed.dae.EntityID
import org.jetbrains.exposed.dae.UUIDEntity
import org.jetbrains.exposed.dae.UUIDEntityClass
import org.jetbrains.exposed.dae.UUIDTable
import org.jetbrains.exposed.sql.SchemaUtils

object Ledgers : UUIDTable("ledger") {
    val householdId = reference("household_id", Households).uniqueIndex()
    val rootAccountId = reference("root_account_id", Accounts).uniqueIndex()
}

class Ledger(id: EntityID<UUID>) : UUIDEntity(id) {
    companion object : UUIDEntityClass<Ledger>(Ledgers)
    
    var household by Household referencedOn Ledgers.householdId
    var rootAccount by Account referencedOn Ledgers.rootAccountId
}

fun setupLedgerTables() : Unit {
    setupCurrencyTable()
    SchemaUtils.create(Accounts, Ledgers)
}