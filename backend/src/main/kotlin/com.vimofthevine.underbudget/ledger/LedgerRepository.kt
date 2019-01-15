package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.auth.UserTable
import com.vimofthevine.underbudget.DbService

import java.util.UUID

import org.jetbrains.exposed.dao.*
import org.jetbrains.exposed.sql.*
import org.joda.time.DateTime

fun DbService.createLedgerPermission(ledger: UUID, user: UUID): UUID =
	LedgerPermissionTable.insertAndGetId {
        it[ledgerId] = EntityID<UUID>(ledger, LedgerTable)
        it[userId] = EntityID<UUID>(user, UserTable)
    }.value

fun DbService.hasLedgerPermission(ledgerId: UUID, userId: UUID): Boolean =
	LedgerPermissionTable.select {
        (LedgerPermissionTable.ledgerId eq ledgerId) and (LedgerPermissionTable.userId eq userId)
    }.count() != 0

fun DbService.deleteLedgerPermission(ledgerId: UUID, userId: UUID) =
	LedgerPermissionTable.deleteWhere {
        (LedgerPermissionTable.ledgerId eq ledgerId) and (LedgerPermissionTable.userId eq userId)
    }

fun DbService.createLedger(ledger: Ledger): UUID =
	LedgerTable.insertAndGetId {
        it[name] = ledger.name
        it[defaultCurrency] = Currencies.get(ledger.defaultCurrency).getNumericCode()
    }.value

fun DbService.toLedger(row: ResultRow) = Ledger (
    id = row[LedgerTable.id].value,
    name = row[LedgerTable.name],
    defaultCurrency = Currencies.get(row[LedgerTable.defaultCurrency]).getCurrencyCode()
)

fun DbService.findLedgerById(id: UUID): Ledger? =
	LedgerTable.select { LedgerTable.id eq id }.mapNotNull { toLedger(it) }.singleOrNull()

fun DbService.findLedgersByUser(userId: UUID): List<Ledger> =
	LedgerTable.innerJoin(LedgerPermissionTable)
		.slice(LedgerTable.columns)
		.select { LedgerPermissionTable.userId eq userId }
		.withDistinct()
		.mapNotNull { toLedger(it) }

