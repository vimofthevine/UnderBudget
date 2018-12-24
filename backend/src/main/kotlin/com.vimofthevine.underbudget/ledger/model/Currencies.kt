package com.vimofthevine.underbudget.ledger.model

import com.vimofthevine.underbudget.Database

import java.util.UUID

import org.jetbrains.squash.connection.Transaction
import org.jetbrains.squash.definition.TableDefinition
import org.jetbrains.squash.definition.autoIncrement
import org.jetbrains.squash.definition.primaryKey
import org.jetbrains.squash.definition.uuid
import org.jetbrains.squash.definition.varchar

object Currencies : TableDefinition("currency") {
    val id = uuid("id").primaryKey()
    val code = varchar("code", 10)
    val externalId = varchar("ext_id", 128)
}

data class Currency(val id : UUID, val code : String, val extId : String?)
