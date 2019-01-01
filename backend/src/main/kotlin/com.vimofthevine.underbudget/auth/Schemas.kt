package com.vimofthevine.underbudget.auth

import java.util.UUID

import org.jetbrains.exposed.dao.UUIDTable
import org.jetbrains.exposed.sql.SchemaUtils

object Users : UUIDTable("user") {
    val name = varchar("name", 128).uniqueIndex()
    val email = varchar("email", 255).uniqueIndex()
    val salt = varchar("salt", 256)
    val password = varchar("password", 256)
    val verified = bool("verified")
    val created = datetime("created")
    val last_updated = datetime("last_updated")
}

fun setupAuthTables() : Unit {
    SchemaUtils.create(Users)
}