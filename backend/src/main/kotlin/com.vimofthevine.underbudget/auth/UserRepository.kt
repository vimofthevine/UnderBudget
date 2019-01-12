package com.vimofthevine.underbudget.auth

import com.vimofthevine.underbudget.DbService

import java.util.UUID

import org.jetbrains.exposed.sql.*
import org.joda.time.DateTime

fun DbService.createUser(user: User): UUID =
	UserTable.insertAndGetId {
        it[name] = user.name
        it[email] = user.email
        it[salt] = user.salt
        it[password] = user.hashedPassword
        it[verified] = false
        it[created] = DateTime()
        it[lastUpdated] = DateTime()
    }.value

fun DbService.toUser(row: ResultRow) = User(
    id = row[UserTable.id].value,
    name = row[UserTable.name],
    email = row[UserTable.email],
    salt = row[UserTable.salt],
    hashedPassword = row[UserTable.password]
)

fun DbService.findUserById(id: UUID): User? =
	UserTable.select { UserTable.id eq id }.mapNotNull { toUser(it) }.singleOrNull()

fun DbService.findUserByName(name: String): User? =
	UserTable.select { UserTable.name eq name }.mapNotNull { toUser(it) }.singleOrNull()

fun DbService.findUserByEmail(email: String): User? =
	UserTable.select { UserTable.email eq email }.mapNotNull { toUser(it) }.singleOrNull()
