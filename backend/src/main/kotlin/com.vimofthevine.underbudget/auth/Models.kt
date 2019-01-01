package com.vimofthevine.underbudget.auth

import com.vimofthevine.underbudget.DbService

import java.util.UUID

import org.jetbrains.exposed.sql.*
import org.joda.time.DateTime

data class User(val name: String, val email: String, val salt: String?, val password: String?)

fun DbService.createUser(newName: String, newEmail: String, newSalt: String,
                         newPassword: String): UUID =
	Users.insertAndGetId {
        it[name] = newName
        it[email] = newEmail
        it[salt] = newSalt
        it[password] = newPassword
        it[verified] = false
        it[created] = DateTime()
        it[last_updated] = DateTime()
    }.value

fun DbService.getUser(id: UUID): User? =
	Users.select { Users.id eq id }.mapNotNull { toUser(it) }.singleOrNull()

fun DbService.findUserByName(name: String): User? =
	Users.select { Users.name eq name }.mapNotNull { toUser(it) }.singleOrNull()

fun DbService.findUserByEmail(email: String): User? =
	Users.select { Users.email eq email }.mapNotNull { toUser(it) }.singleOrNull()

fun DbService.toUser(row: ResultRow) = User(
    name = row[Users.name],
    email = row[Users.email],
    salt = row[Users.salt],
    password = row[Users.password]
)