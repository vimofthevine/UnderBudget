package com.vimofthevine.underbudget.auth

import com.vimofthevine.underbudget.DbService

import java.util.UUID

import org.jetbrains.exposed.dao.*
import org.jetbrains.exposed.sql.*
import org.joda.time.DateTime

fun DbService.createToken(token: Token): UUID =
	TokenTable.insertAndGetId {
        it[jwtId] = token.jwtId
        it[userId] = EntityID<UUID>(token.userId, UserTable)
        it[issued] = token.issued
        it[subject] = token.subject
    }.value

fun DbService.toToken(row: ResultRow) = Token(
	id = row[TokenTable.id].value,
    jwtId = row[TokenTable.jwtId],
    userId = row[TokenTable.userId].value,
    issued = row[TokenTable.issued],
    subject = row[TokenTable.subject]
)

fun DbService.findTokenByJwtId(jwtId: String): Token? =
	TokenTable.select { TokenTable.jwtId eq jwtId }.mapNotNull { toToken(it) }.singleOrNull()

fun DbService.findTokensByUser(userId: UUID): List<Token> =
	TokenTable.select { TokenTable.userId eq userId }.mapNotNull { toToken(it) }

fun DbService.deleteTokenByJwtId(jwtId: String) =
	TokenTable.deleteWhere { TokenTable.jwtId eq jwtId }
