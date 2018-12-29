package com.vimofthevine.underbudget.ledger

import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

fun Routing.ledger(db: Database, auth: Boolean) {
    ledgerEndpoints(db, auth)
    accountEndpoints(db)
    envelopeEndpoints(db)
}