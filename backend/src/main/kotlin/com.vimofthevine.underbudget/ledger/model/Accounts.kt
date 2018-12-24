package com.vimofthevine.underbudget.ledger.model

import org.jetbrains.squash.definition.TableDefinition
import org.jetbrains.squash.definition.autoIncrement
import org.jetbrains.squash.definition.bool
import org.jetbrains.squash.definition.default
import org.jetbrains.squash.definition.integer
import org.jetbrains.squash.definition.reference
import org.jetbrains.squash.definition.primaryKey
import org.jetbrains.squash.definition.uuid
import org.jetbrains.squash.definition.varchar

object Accounts: TableDefinition("account") {
    val id = uuid("id").primaryKey()
    val name = varchar("name", 128)
    val currencyId = reference(Currencies.id, "currency_id")
    val archived = bool("archived").default(false)
    val externalId = varchar("ext_id", 32)
    val parentId = reference(id, "parent_id")
}