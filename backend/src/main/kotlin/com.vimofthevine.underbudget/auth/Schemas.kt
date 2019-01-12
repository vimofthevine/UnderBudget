package com.vimofthevine.underbudget.auth

import java.util.UUID

import org.jetbrains.exposed.dao.UUIDTable
import org.jetbrains.exposed.sql.SchemaUtils

object UserTable : UUIDTable("user") {
    val name = varchar("name", 128).uniqueIndex()
    val email = varchar("email", 256).uniqueIndex()
    val salt = varchar("salt", 256)
    val password = varchar("password", 256)
    val verified = bool("verified")
    val created = datetime("created")
    val lastUpdated = datetime("last_updated")
}

object TokenTable : UUIDTable("token") {
    val jwtId = varchar("jwt_id", 256).uniqueIndex()
    val userId = reference("user_id", UserTable)
    val issued = datetime("issued")
    val subject = varchar("subject", 512)
}

fun setupAuthTables() : Unit {
    SchemaUtils.create(UserTable, TokenTable)
}