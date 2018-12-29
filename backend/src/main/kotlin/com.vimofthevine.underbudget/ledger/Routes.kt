package com.vimofthevine.underbudget.ledger

import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

fun Routing.ledger(db: Database) {
    ledgerEndpoints(db)
    accountEndpoints(db)
    envelopeEndpoints(db)
}