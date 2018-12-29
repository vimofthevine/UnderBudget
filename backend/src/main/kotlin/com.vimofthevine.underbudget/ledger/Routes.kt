package com.vimofthevine.underbudget.ledger

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

fun Routing.ledger(db: Database) {
    get("/accounts") {
        call.respondText("Accounts page", ContentType.Text.Html)
    }
    get("/envelopes") {
        call.respondText("Envelopes page", ContentType.Text.Html)
    }
    get("/transactions") {
        call.respondText("Transactions page", ContentType.Text.Html)
    }
}