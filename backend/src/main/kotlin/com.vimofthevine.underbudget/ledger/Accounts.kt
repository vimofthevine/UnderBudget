package com.vimofthevine.underbudget.ledger

import java.util.UUID

import org.jetbrains.exposed.dao.EntityID
import org.jetbrains.exposed.dao.UUIDEntity
import org.jetbrains.exposed.dao.UUIDEntityClass
import org.jetbrains.exposed.dao.UUIDTable

object Accounts : UUIDTable("account") {
    val parentId = reference("parent_id", Accounts).nullable()
    val ledgerId = reference("ledger_id", Ledgers)
    val name = varchar("name", 128)
    val currency = integer("currency")
    val archived = bool("archived").default(false)
    val time = datetime("time")
    val externalId = varchar("ext_id", 32).nullable()
    val institution = varchar("institution", 128).nullable()
    val number = varchar("number", 32).nullable()
}

class Account(id: EntityID<UUID>) : UUIDEntity(id) {
    companion object : UUIDEntityClass<Account>(Accounts)
    
    var parent by Account optionalReferencedOn Accounts.parentId
    val children by Account optionalReferrersOn Accounts.parentId
    var ledger by Ledger referencedOn Accounts.ledgerId
    var name by Accounts.name
    private var currencyCode by Accounts.currency
    var currency
    	get() = Currencies.get(currencyCode)
    	set(value) { currencyCode = value.getNumericCode() }
    var archived by Accounts.archived
    var time by Accounts.time
    var externalId by Accounts.externalId
    var institution by Accounts.institution
    var number by Accounts.number
}