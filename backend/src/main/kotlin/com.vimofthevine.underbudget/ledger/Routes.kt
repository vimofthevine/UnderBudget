package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.DbService

import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

fun Routing.ledger(db: DbService, auth: Boolean) {
    ledgerEndpoints(db, auth)
    //accountEndpoints(db)
    //envelopeEndpoints(db)
}