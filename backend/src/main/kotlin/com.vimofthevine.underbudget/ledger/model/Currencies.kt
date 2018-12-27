package com.vimofthevine.underbudget.ledger.model

import com.vimofthevine.underbudget.Database

import java.util.UUID

import javax.money.Monetary

import org.jetbrains.exposed.dao.EntityID
import org.jetbrains.exposed.dao.UUIDEntity
import org.jetbrains.exposed.dao.UUIDEntityClass
import org.jetbrains.exposed.dao.UUIDTable
import org.jetbrains.exposed.sql.SchemaUtils

object Currencies : UUIDTable("currency") {
    val code = varchar("code", 10)
    val externalId = varchar("ext_id", 128)
}

class Currency(id: EntityID<UUID>) : UUIDEntity(id) {
    companion object : UUIDEntityClass<Currency>(Currencies)
    
    var code by Currencies.code
    var externalId by Currencies.externalId
    val unit by lazy {
        Monetary.getCurrency(code)
    }
}

fun setupCurrencies() : Unit {
    SchemaUtils.create(Currencies)
    if (Currencies.selectAll().count() == 0) {
        Currencies.insert 
    }
}