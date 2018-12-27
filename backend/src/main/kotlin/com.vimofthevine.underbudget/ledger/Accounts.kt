package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.auth.Household
import com.vimofthevine.underbudget.auth.Households

import java.util.UUID

import org.jetbrains.exposed.dao.EntityID
import org.jetbrains.exposed.dao.UUIDEntity
import org.jetbrains.exposed.dao.UUIDEntityClass
import org.jetbrains.exposed.dao.UUIDTable

object Accounts : UUIDTable("account") {
    val householdId = reference("household_id", Households)
    val name = varchar("name", 128)
    val currencyId = reference("currency_id", Currencies)
    val archived = bool("archived").default(false)
    val externalId = varchar("ext_id", 32)
    val parentId = reference("parent_id", Accounts)
}

class Account(id: EntityID<UUID>) : UUIDEntity(id) {
    companion object : UUIDEntityClass<Account>(Accounts)
    
    var household by Household referencedOn Accounts.householdId
    var name by Accounts.name
    var currency by Currency referencedOn Accounts.currencyId
    var archived by Accounts.archived
    var externalId by Accounts.externalId
    var parent by Account referencedOn Accounts.parentId
}