package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.DbService

import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

fun Route.ledger(db: DbService) {
    ledgerRoutes(db)
    //accountEndpoints(db)
    //envelopeEndpoints(db)
}