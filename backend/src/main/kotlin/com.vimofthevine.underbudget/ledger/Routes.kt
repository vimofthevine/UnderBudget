package com.vimofthevine.underbudget.ledger

import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

fun Routing.ledger(db: Database, service: LedgerService, auth: Boolean) {
    ledgerEndpoints(db, service, auth)
    accountEndpoints(db)
    envelopeEndpoints(db)
}