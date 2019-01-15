package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.auth.UserTable

import java.util.UUID

import org.jetbrains.exposed.dao.UUIDTable
import org.jetbrains.exposed.sql.SchemaUtils
import org.joda.time.DateTime

object LedgerTable : UUIDTable("ledger") {
    val name = varchar("name", 128)
    val defaultCurrency = integer("default_currency")
    val created = datetime("created").clientDefault { DateTime() }
    val lastUpdated = datetime("last_updated").clientDefault { DateTime() }
}

object LedgerPermissionTable : UUIDTable("ledger_permission") {
    val ledgerId = reference("ledger_id", LedgerTable)
    val userId = reference("user_id", UserTable)
}

object AccountTable : UUIDTable("account") {
    val parentId = reference("parent_id", AccountTable).nullable()
    val ledgerId = reference("ledger_id", LedgerTable)
    val name = varchar("name", 128)
    val currency = integer("currency")
    val archived = bool("archived").default(false)
    val created = datetime("created").clientDefault { DateTime() }
    val lastUpdated = datetime("last_updated").clientDefault { DateTime() }
    val externalId = varchar("ext_id", 32).nullable()
    val institution = varchar("institution", 128).nullable()
    val number = varchar("number", 32).nullable()
}

object EnvelopeTable : UUIDTable("envelope") {
    val parentId = reference("parent_id", EnvelopeTable).nullable()
    val ledgerId = reference("ledger_id", LedgerTable)
    val name = varchar("name", 128)
    val currency = integer("currency")
    val archived = bool("archived").default(false)
    val created = datetime("created").clientDefault { DateTime() }
    val lastUpdated = datetime("last_updated").clientDefault { DateTime() }
    val externalId = varchar("ext_id", 32).nullable()
}

fun setupLedgerTables() : Unit {
    SchemaUtils.create(AccountTable, EnvelopeTable, LedgerPermissionTable, LedgerTable)
}
