package com.vimofthevine.underbudget.ledger

import java.util.UUID

import org.jetbrains.exposed.dao.EntityID
import org.jetbrains.exposed.dao.UUIDEntity
import org.jetbrains.exposed.dao.UUIDEntityClass
import org.jetbrains.exposed.dao.UUIDTable

object Envelopes : UUIDTable("envelope") {
    val parentId = reference("parent_id", Envelopes).nullable()
    val ledgerId = reference("ledger_id", Ledgers)
    val name = varchar("name", 128)
    val currency = integer("currency")
    val archived = bool("archived").default(false)
    val time = datetime("time")
    val externalId = varchar("ext_id", 32).nullable()
}

class Envelope(id: EntityID<UUID>) : UUIDEntity(id) {
    companion object : UUIDEntityClass<Envelope>(Envelopes)
    
    var parent by Envelope optionalReferencedOn Envelopes.parentId
    val children by Envelope optionalReferrersOn Envelopes.parentId
    var ledger by LedgerDao referencedOn Envelopes.ledgerId
    var name by Envelopes.name
    private var currencyCode by Envelopes.currency
    var currency
    	get() = Currencies.get(currencyCode)
    	set(value) { currencyCode = value.getNumericCode() }
    var archived by Envelopes.archived
    var time by Envelopes.time
    var externalId by Envelopes.externalId
}